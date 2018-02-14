#include <stdio.h>		//printf
#include <stdlib.h>		//exit
#include <unistd.h>		//fork
#include <pthread.h>
#include <semaphore.h>


#define SHARED 0


sem_t sem[6];


struct threads
{
   int id;
   int sec;
   int signal[6];
};

void* tfunc(void *arg) 
{
   struct threads *targs=arg;
   sem_wait(&sem[targs->id]);
   printf("Hei. Tråd %d har startet\n", targs->id);
   printf("Den skal kjøre i %d sekund\n", targs->sec);
   sleep(targs->sec);
   for(int i = 0; i < 6; i++)
   {
      if (targs->signal[i] == 1)
      {
         sem_post(&sem[i]);
      }
   }
}

int main(void)
{
   int i, j;
   struct threads* targs[6];
   pthread_t tid[6];


   for(j = 0; j < 6; j++)
   {
      targs[j] = malloc(sizeof(struct threads));
      targs[j]->id = 0;
      targs[j]->sec = 0;
      for(i = 0; i < 6; i++)
      {
          targs[j]->signal[i] = 0;
      }
   }

//START T0
   targs[0]->id = 0;
   targs[0]->sec = 1;
   targs[0]->signal[1] = 1;		//Signaliserer tråd1
   targs[0]->signal[4] = 1;		//signaliserer tråd4
   sem_init(&sem[0], SHARED, 1);	//initialiserer semafor til T0
   pthread_create(&tid[0], NULL, tfunc, targs[0]);	//starter T0

//START T1
   targs[1]->id = 1;
   targs[1]->sec = 2;
   targs[1]->signal[3] = 1;		//signaliserer tråd3
   sem_init(&sem[1], SHARED, 0);        //initialiserer semafor til T0
   pthread_create(&tid[1], NULL, tfunc, targs[1]);      //starter T0

//START T2
   targs[2]->id = 2;
   targs[2]->sec = 3;
   sem_init(&sem[2], SHARED, 1);        //initialiserer semafor til T0
   pthread_create(&tid[2], NULL, tfunc, targs[2]);      //starter T0

//START T3
   targs[3]->id = 3;
   targs[3]->sec = 2;
   sem_init(&sem[3], SHARED, 0);        //initialiserer semafor til T0
   pthread_create(&tid[3], NULL, tfunc, targs[3]);      //starter T0

//START T4
   targs[4]->id = 4;
   targs[4]->sec = 3;
   targs[4]->signal[5] = 1;
   sem_init(&sem[4], SHARED, 0);        //initialiserer semafor til T0
   pthread_create(&tid[4], NULL, tfunc, targs[4]);      //starter T0

//START T5
   targs[5]->id = 5;
   targs[5]->sec = 3;
   sem_init(&sem[5], SHARED, 0);        //initialiserer semafor til T0
   pthread_create(&tid[5], NULL, tfunc, targs[5]);      //starter T0

   pthread_join(tid[0], NULL);
   pthread_join(tid[1], NULL);
   pthread_join(tid[2], NULL);
   pthread_join(tid[3], NULL);
   pthread_join(tid[4], NULL);
   pthread_join(tid[5], NULL);

   return 0;
}


