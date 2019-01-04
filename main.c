#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <stdio.h>
#include <time.h>

#include "pt-sem.h"

#define N 5                      //Quantidade de filósofos
#define ESQUERDA (i + N - 1) % N //Id do filósofo a esquerda do id
#define DIREITA (i + 1) % N      //Id do filósofo a direita do id
#define PENSANDO 0               //Id para estado pensado
#define FAMINTO 1                //Id para estado de fome
#define COMENDO 2                //Id para estado comendo


/* 
@brief macro/funcao de espera pensar, passa a vez enquanto nao atinge determinado tempo 
@param pt parametro do protothread em operacao, i id de identificacao do filosofo
Line 1 atribui um numero aleatorio de segundos de 1 a 10
Line 3 atribui tempo inicial
Line 4 protothread entre em espera e passa a vez enquanto o tempo entre o inicio e o corrente nao atinge r segundos
 */
#define PENSAR(pt, i)   \
    r = (rand() %10 + 1);   \
    printf("Filosofo %d pensa por %d segundos\n", i+1, r); \
    inicio= time(NULL);    \
    PT_WAIT_WHILE(pt, (time(NULL) - inicio) < r);   \


/* 
@brief macro/funcao de espera comer, passa a vez enquanto nao atinge determinado tempo 
@param pt parametro do protothread em operacao, i id de identificacao do filosofo
Line 1 atribui um numero aleatorio de segundos de 1 a 10
Line 3 atribui tempo inicial
Line 4 protothread entre em espera e passa a vez enquanto o tempo entre o inicio e o corrente nao atinge r segundos
 */
#define COMER(pt, i)    \
    r = (rand() %10 + 1);   \
    printf("Filosofo %d come por %d segundos\n", i+1, r); \
    inicio= time(NULL);    \
    PT_WAIT_WHILE(pt, (time(NULL) - inicio) < r); \



/* 
@brief macro/funcao intencao do filosofo, verifica se os visinhos nao estao comendo e se o filosofo corrente
esta faminto e entao da a vez do filosofo comer
@param pt parametro do protothread em operacao, i id de identificacao do filosofo
Line 6 desbloqueia mutex do filosofo  
*/
#define INTENCAO(pt, i) \
    printf("Filosofo %d esta com intencao de comer\n", i+1);  \
    if ((estado[i] == FAMINTO) && (estado[ESQUERDA] != COMENDO) && (estado[DIREITA] != COMENDO))    \
    {      \
        printf("Filosofo %d ganhou a vez de comer\n", i+1);   \
        estado[i] = COMENDO;    \
        PT_SEM_SIGNAL(pt, &(mux_f[i]));    \
    }\




/*
@brief macro pegar garfo segunda linha
*/
#define PEGAR_GARFO_2L PT_SEM_WAIT(pt, &(mux_f[i]));\



/* 
@brief macro/funcao pegar garfo, muda estado para faminto, inicia intencao de comer e tentar pegar os dois garfos 
@param pt parametro do protothread em operacao, i id de identificacao do filosofo
Line 1 bloqueia mutex regiao critica, caso negado passa a vez
Line 2 seta estado faminto
Line 3 inicia intenção de comer
Line 4 desbloqueia mutex regiao critica
*/
#define PEGAR_GARFO(pt, i)  \
    PT_SEM_WAIT(pt, &mutex);    \ 
    estado[i] = FAMINTO;    \
    INTENCAO(pt, i);    \
    PT_SEM_SIGNAL(pt, &mutex);  \ 




/* 
@brief macro/funcao por garfo, muda estado para pensando, da a chance de seus vizinhos comerem atravez da intencao 
@param pt parametro do protothread em operacao, i id de identificacao do filosofo
Line 1 bloqueia mutex regiao critica, caso negado passa a vez
Line 3 seta estado pensando
Line 4 inicia intenção de comer do visinho esquerdo
Line 5 incia intenção de comer do visinho direito
Line 6 desbloqueia mutex regiao critica
*/
#define POR_GARFO(pt, i)    \
    PT_SEM_WAIT(pt, &mutex);    \
    printf("Filosofo %d esta pensando\n", i+1);   \
    estado[i] = PENSANDO;   \
    INTENCAO(pt, ESQUERDA); \
    INTENCAO(pt, DIREITA);  \
    PT_SEM_SIGNAL(pt, &mutex);  \



static int estado[N];               // array dos estados filosofos
static struct pt_sem mutex;         // mutex de estrutura semaforo protothread para entrada em rc
static struct pt_sem mux_f[N];      // mutex's de estrutura semaforo protothread para cada filosofo
static struct pt jantar[N];         // array de estrutura protothread representando thread de cada filosofo 
static time_t inicio = 0;         
static int r = 0;




/* 
@brief funcao filosofo, declara o inicio e o fim do swith dos protothread e estabelece o fluxo pensar, 
pegar garfo, comer, por garfo em uma seguencia infinita 
@param pt parametro do protothread em operacao, i id de identificacao do filosofo
Line 4 inicio do swith
Line 11 segunda linha do macro pegar garfo
Line 18 fim do swith
*/
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


int main(void)
{

    int cont;
    PT_SEM_INIT(&mutex, 1);                 // inicializa semaforo rc
    for(cont=0; cont < N; cont++)           
    {                                       // inicializa semaforos e thread filosofos
        PT_SEM_INIT(&(mux_f[cont]), 1);         
        PT_INIT(&(jantar[cont]));
    }
    

    while(1){ 

        filosofo(&(jantar[0]), 0);          // Sleeps(atrasos) de 1 segundo para melhor visualizacao
        #ifdef _WIN32
            Sleep(1000);
        #else
            sleep(1);
        #endif

        filosofo(&(jantar[1]), 1);
        #ifdef _WIN32
            Sleep(1000);
        #else
            sleep(1);
        #endif

        filosofo(&(jantar[2]), 2);
        #ifdef _WIN32
            Sleep(1000);
        #else
            sleep(1);
        #endif

        filosofo(&(jantar[3]), 3);
        #ifdef _WIN32
            Sleep(1000);
        #else
            sleep(1);
        #endif

        filosofo(&(jantar[4]), 4);
        #ifdef _WIN32
            Sleep(1000);
        #else
            sleep(1);
        #endif
    }
    
}