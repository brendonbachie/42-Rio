#!/bin/bash

# Script para testar push_swap em 10000 combinações de 500 números
# Conta movimentos, FAILs e calcula média

FAIL=0
TOTAL=0
ITERATIONS=100
NUMBERS=500
MAX_NUM=999

for i in $(seq 1 $ITERATIONS); do
    # Gera 500 números aleatórios
    ARG=$(shuf -i 0-$MAX_NUM -n $NUMBERS)
    
    # Executa push_swap e conta movimentos
    MOVES=$(./push_swap $ARG | wc -l)
    
    # Soma para cálculo de média
    TOTAL=$((TOTAL + MOVES))
    
    # Checa se excedeu 5500 movimentos
    if [ "$MOVES" -gt 5500 ]; then
        echo -e "\e[31mFAIL\e[0m $i => $MOVES"
        FAIL=$((FAIL+1))
    else
        echo -e "\e[32mOK\e[0m   $i => $MOVES"
    fi
done

# Calcula média de movimentos
MEDIA=$((TOTAL / ITERATIONS))

echo -e "\nTOTAL FAILS: \e[31m$FAIL\e[0m"
echo -e "MEDIA MOVIMENTOS: \e[34m$MEDIA\e[0m"
