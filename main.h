#ifndef __LPC2103_H
#define __LPC2103_H

#include "pt-sem.h"

#define N 5                     
#define ESQUERDA (i + N - 1) % N 
#define DIREITA (i + 1) % N      
#define PENSANDO 0              
#define FAMINTO 1                
#define COMENDO 2                

#define PENSAR(pt, i) r = (rand() %10 + 1);	printf("Filosofo %d pensa por %d segundos\n", i+1, r); inicio= time(NULL); PT_WAIT_WHILE(pt, (time(NULL) - inicio) < r);	

#define COMER(pt, i) r = (rand() %10 + 1); printf("Filosofo %d come por %d segundos\n", i+1, r); inicio= time(NULL); PT_WAIT_WHILE(pt, (time(NULL) - inicio) < r);


#define INTENCAO(pt, i) printf("Filosofo %d esta com intencao de comer\n", i+1); if ((estado[i] == FAMINTO) && (estado[ESQUERDA] != COMENDO) && (estado[DIREITA] != COMENDO)){ printf("Filosofo %d ganhou a vez de comer\n", i+1); estado[i] = COMENDO; PT_SEM_SIGNAL(pt, &(mux_f[i]));} 


#define PEGAR_GARFO_2L PT_SEM_WAIT(pt, &(mux_f[i]));

		
#define PEGAR_GARFO(pt, i) PT_SEM_WAIT(pt, &mutex); estado[i] = FAMINTO; INTENCAO(pt, i); PT_SEM_SIGNAL(pt, &mutex); 


#define POR_GARFO(pt, i)PT_SEM_WAIT(pt, &mutex); printf("Filosofo %d esta pensando\n", i+1); estado[i] = PENSANDO; INTENCAO(pt, ESQUERDA); INTENCAO(pt, DIREITA);PT_SEM_SIGNAL(pt, &mutex); 

static int estado[N];               
static struct pt_sem mutex;
static struct pt_sem mux_f[N];      
static struct pt jantar[N];         
static time_t inicio = 0;         
static int r = 0;


static PT_THREAD(filosofo(struct pt *pt, int i))
{
    printf("Estou no filosofo %d\n", i+1);

    PT_BEGIN(pt);

    while (1)
    {
        PENSAR(pt, i);

        PEGAR_GARFO(pt, i);
			
        PEGAR_GARFO_2L;

        COMER(pt, i);

        POR_GARFO(pt, i);
    }

    PT_END(pt);
}

#endif
