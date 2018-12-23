/*
PARA COMPILAR

>> gcc filosofos.c -lpthread -o arqexec
>> ./arqexec

*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N         5									//Quantidade de filósofos
#define ESQUERDA	(i + N - 1) % N	//Id do filósofo a esquerda do id
#define DIREITA		(i + 1) % N			//Id do filósofo a direita do id
#define PENSANDO	0									//Id para estado pensado
#define FAMINTO	  1									//Id para estado de fome
#define COMENDO	  2									//Id para estado comendo

int estado[N];
pthread_mutex_t mutex; //região crítica
pthread_mutex_t mux_f[N]; // mutex por filosofo
pthread_t jantar[N];

void * filosofo(void * filosofo){
  int * i = (int *) filosofo;
  printf("Filosofo %d foi criado com sucesso\n", *(i) );

  while (1){
    pensar(*(i));
    pegar_garfo(*(i));
    comer(*(i));
    por_garfo(*(i));
  }
  pthread_exit((void*) 0);
}

void pegar_garfo(int i){
  pthread_mutex_lock(&(mutex)); // entra na regiao critica
  estado[i] = FAMINTO;
  intencao(i);
  pthread_mutex_unlock(&(mutex));
  pthread_mutex_lock(&(mux_f[i]));
}
void por_garfo(int i){
  pthread_mutex_lock(&mutex);
  printf("Filosofo %d esta pensando\n", i);
  estado[i] = PENSANDO;
  intencao(ESQUERDA);
  intencao(DIREITA);
  pthread_mutex_unlock(&mutex);
}
void intencao(int i){
  printf("Filosofo %d esta com intencao de comer\n", i);
  if((estado[i] == FAMINTO) && (estado[ESQUERDA] != COMENDO) && (estado[DIREITA] != COMENDO )){
    printf("Filosofo %d ganhou a vez de comer\n", i);
    estado[i] = COMENDO;
    pthread_mutex_unlock(&(mux_f[i]));
  }
}
void pensar(int i){
  int r = (rand() % 10 + 1);
  printf("Filosofo %d pensa por %d segundos\n", i,r);
  sleep(r);
}
void comer(int i){
  int r = (rand() % 10 + 1);
  printf("Filosofo %d come por %d segundos\n",i,r);
  sleep(r);
}

int main(int argc, char const *argv[]) {
  int cont; // contador auxiliar
  int status; // criação da thread

  pthread_mutex_init(&mutex,NULL); //inicia os mutexes
  for(cont = 0; cont < N; cont++){
    pthread_mutex_init(&(mux_f[cont]),NULL);
  }

  for(cont = 0; cont < N; cont++){//inicia threads dos filósofos
    status = pthread_create(&(jantar[cont]),NULL,filosofo,(void *) &(cont));
    if(status){
      printf("Erro criando thread %d, retornou codigo %d\n",cont,status);
      return EXIT_FAILURE;
    }

  }

  pthread_mutex_destroy(&(mutex));
  for(cont=0;cont < N;cont++){
    pthread_mutex_destroy(&(mux_f[cont]));
  }
  pthread_exit(NULL);

  return EXIT_SUCCESS;
}
