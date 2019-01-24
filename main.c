#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "main.h"
#include "lpc2103.h"

int main(void)
{

    int cont = 0;
    PT_SEM_INIT(&mutex, 1);                 
    for(cont=0; cont < N; cont++)           
    {                                       
        PT_SEM_INIT(&(mux_f[cont]), 1);         
        PT_INIT(&(jantar[cont]));
    }
    

    while(1){ 

        filosofo(&(jantar[0]), 0);          
      

        filosofo(&(jantar[1]), 1);
      

        filosofo(&(jantar[2]), 2);
    

        filosofo(&(jantar[3]), 3);
      

        filosofo(&(jantar[4]), 4);
     
    }
    
}


