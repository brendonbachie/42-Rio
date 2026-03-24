import os
from fastapi import FastAPI, Request, Form
import requests
from fastapi.responses import JSONResponse
from fastapi.templating import Jinja2Templates
from fastapi.staticfiles import StaticFiles
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel
import secrets

from dotenv import load_dotenv

load_dotenv()  # Carrega variáveis de ambiente do arquivo .env

app = FastAPI(title="Captive Portal API")

# Templates and static files
templates = Jinja2Templates(directory="templates")
app.mount("/static", StaticFiles(directory="static"), name="static")

# CORS - adjust origins in production
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
    """Renderiza a página de login (formulário + botão do provider 42)."""
    return templates.TemplateResponse("login.html", {"request": request})


@app.post("/auth/login")
async def auth_login(payload: LoginRequest):
    """Recebe JSON { username, password } e retorna um token de exemplo.
    Substitua a verificação por integração real com sua base de usuários.
    """
    # TODO: validar credenciais contra DB / sistema real
    if payload.username == "admin" and payload.password == "password":
        token = secrets.token_urlsafe(32)
        return {"access_token": token, "token_type": "bearer"}
    return JSONResponse(status_code=401, content={"detail": "Credenciais inválidas"})


@app.post("/auth/login-form")
async def auth_login_form(username: str = Form(...), password: str = Form(...)):
    """Recebe submissão de formulário (x-www-form-urlencoded) e delega para o handler JSON."""
    payload = LoginRequest(username=username, password=password)
    return await auth_login(payload)

@app.get("/login/42")
async def auth_42(request: Request):
    """faça o request de autenticação com a 42 para pegar os dados do usuário."""
    base_url = os.getenv("BASE_URL_42", "https://auth.42.fr")

    def get_token(code: str):
        token_url = f"{base_url}/oauth/token"
        data = {
            "grant_type": "authorization_code",
            "client_id": os.getenv("CLIENT_ID_42"),
            "client_secret": os.getenv("CLIENT_SECRET_42"),
            "redirect_uri": "http://localhost:8000/login/42",
            "code": code
        }
        response = requests.post(token_url, data=data)
        print("Token response:", response.status_code, response.text)  # Debug
        return response.json()

    def get_user_info(access_token: str):
        user_url = f"{base_url}/v2/me"
        headers = {"Authorization": f"Bearer {access_token}"}
        response = requests.get(user_url, headers=headers)
        return response.json()

    code = request.query_params.get("code")
    if not code:
        return JSONResponse(status_code=400, content={"detail": "Código de autorização ausente"})

    token_response = get_token(code)
    access_token = token_response.get("access_token")
    if not access_token:
        return JSONResponse(status_code=401, content={"detail": "Falha ao obter token de acesso"})

    user_info = get_user_info(access_token)
    return user_info