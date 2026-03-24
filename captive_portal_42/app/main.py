import os
import json
import csv
from datetime import datetime
from fastapi import FastAPI, Request, Form
from fastapi.responses import JSONResponse, RedirectResponse, HTMLResponse
from fastapi.templating import Jinja2Templates
from fastapi.staticfiles import StaticFiles
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel
import requests
import secrets
from dotenv import load_dotenv

load_dotenv()

app = FastAPI(title="Captive Portal API")

@app.middleware("http")
async def add_ngrok_header(request: Request, call_next):
    response = await call_next(request)
    response.headers["ngrok-skip-browser-warning"] = "true"
    return response

templates = Jinja2Templates(directory="templates")
app.mount("/static", StaticFiles(directory="static"), name="static")

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

class LoginRequest(BaseModel):
    username: str
    password: str

@app.get("/")
async def index(request: Request):
    return templates.TemplateResponse("login.html", {"request": request})

@app.post("/auth/login")
async def auth_login(payload: LoginRequest):
    if payload.username == "admin" and payload.password == "password":
        token = secrets.token_urlsafe(32)
        return {"access_token": token, "token_type": "bearer"}
    return JSONResponse(status_code=401, content={"detail": "Credenciais inválidas"})

@app.post("/auth/login-form")
async def auth_login_form(username: str = Form(...), password: str = Form(...)):
    payload = LoginRequest(username=username, password=password)
    return await auth_login(payload)

@app.get("/login/42")
async def auth_42(request: Request):
    base_url = os.getenv("BASE_URL_42", "https://api.intra.42.fr")

    def get_token(code: str):
        token_url = f"{base_url}/oauth/token"
        data = {
            "grant_type": "authorization_code",
            "client_id": os.getenv("CLIENT_ID_42"),
            "client_secret": os.getenv("CLIENT_SECRET_42"),
            "redirect_uri": os.getenv("REDIRECT_URI_42", "http://localhost:8000/login/42"),
            "code": code
        }
        response = requests.post(token_url, data=data)
        print("Token response:", response.status_code, response.text)
        return response.json()

    def get_user_info(access_token: str):
        user_url = f"{base_url}/v2/me"
        headers = {"Authorization": f"Bearer {access_token}"}
        response = requests.get(user_url, headers=headers)
        return response.json()

    def salvar_metrica(username: str, ip: str):
        with open("logins.csv", "a", newline="") as f:
            csv.writer(f).writerow([datetime.now().isoformat(), username, ip])

    code = request.query_params.get("code")
    if not code:
        return JSONResponse(status_code=400, content={"detail": "Código de autorização ausente"})

    state = request.query_params.get("state", "{}")
    try:
        pfsense = json.loads(state)
    except Exception:
        pfsense = {}

    action   = pfsense.get("action", "")
    redirurl = pfsense.get("redirurl", "http://www.google.com")
    zone     = pfsense.get("zone", "")

    token_response = get_token(code)
    access_token = token_response.get("access_token")
    if not access_token:
        return JSONResponse(status_code=401, content={"detail": "Falha ao obter token de acesso"})

    user_info = get_user_info(access_token)
    username = user_info.get("login", "unknown")
    user_ip  = request.client.host

    print(f"Usuário autenticado: {username} | IP ngrok: {user_ip}")

    salvar_metrica(username, user_ip)

    # Faz o browser do Ubuntu fazer o POST diretamente pro pfSense
    # Assim o pfSense vê o IP correto do Ubuntu, não o IP do ngrok
    if action:
        return HTMLResponse(content=f"""
<!DOCTYPE html>
<html>
<head><meta charset="UTF-8"></head>
<body>
  <form id="f" method="post" action="{action}">
    <input type="hidden" name="accept"   value="Continue">
    <input type="hidden" name="redirurl" value="{redirurl}">
    <input type="hidden" name="zone"     value="{zone}">
  </form>
  <script>document.getElementById('f').submit();</script>
</body>
</html>
""")

    return RedirectResponse(url=redirurl)