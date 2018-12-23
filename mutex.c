#include <iostream>
using namespace std;

int var_bloqueio = 0;

void liberar()
{
    __asm  
    {
        mov eax, 1   
        xchg eax, var_bloqueio  
    }
}

void bloquear()
{
    __asm
    {
        mov eax, 0
        xchg eax, var_bloqueio
    }
}