#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     /* usleep */
#include <pthread.h>
#define SHARED 0        /* process-sharing if !=0, thread-sharing if =0*/
#define BUF_SIZE 10
#define MAX_MOD 100000
#define NUM_ITER 200

void *Producer(void *); /* Producer thread */
void *Consumer(void *); /* Consumer thread */

pthread_cond_t condp;   /* condp: Variable to wait on if buffer full */
pthread_cond_t condc;   /* condc: Variable to wait on if buffer empty */
pthread_mutex_t mutex;  /* mutex: Mutex lock */
int g_data[BUF_SIZE];   /* shared finite buffer  */
int g_idx;              /* index to next available slot in buffer,
                           remember that globals are set to zero
                           according to C standard, so no init needed  */

int main(int argc, char *argv[]) {

	if (argc != 2) {             //sjekker at antall argumenter stemmer
		printf("Det er noe feil med argumentene");
		return -1;
	}
	int ant = atoi(argv[1]);          //får *argv inn i en int

	if (ant < 1) {             //sjekker at argumentet ikke er et negativt tall
		printf("Kun positive tall");
		return -1;
	}

	pthread_t pid[ant], cid[ant];           //endret til array for X ant Con/Prod

	// Initialize the contition-variables and the mutex.
	pthread_cond_init(&condp, 0);
	pthread_cond_init(&condc, 0);
	pthread_mutex_init(&mutex,0);

	// Spawn the two threads

	int i;           //variabel til for-løkker

	printf("main started\n");


	for (i = 0; i < ant; i++){          //for antall ønskede producere/consumere
	  int *nr;                              //argument som skal sendes med til funksjon
	  nr = malloc(sizeof(int));             //reserverer minneplass
	  *nr = i;
	  pthread_create(&pid[i], NULL, Producer, nr);   //sender med argument
	  pthread_create(&cid[i], NULL, Consumer, nr);   //sender med argument
	}
	// Wait for the threads to finish before exiting.
	for (i = 0; i< ant; i++) {
	  pthread_join(pid[i], NULL);
	  pthread_join(cid[i], NULL);
	}

	printf("main done\n");
	return 0;
}


void *Producer(void *arg) {

	int i=0,j;
	int id = *(int*)arg;          //caster om til int igjen

	while(i < NUM_ITER) {
		// Pretend to generate someting, by a random wait.
		usleep(rand()%MAX_MOD);

		// Enter critical region, with exclusive access.
		pthread_mutex_lock(&mutex);

		// While there is no space for more in the buffer, wait on
		// the producer's condition-variable.
		while(g_idx==BUF_SIZE-1)
			pthread_cond_wait(&condp,&mutex);

		// Insert the item to the buffer.
		g_data[g_idx]=1;
		g_idx++;

		// Print the buffer-status.            // Lagt inn id for producer
		j=0; printf("(Producer %d, buffer index is %d)\t",id, g_idx);
		while(j < g_idx) { j++; printf("="); } printf("\n");

		// Signal threads waiting on the consumer conditionvariable
		// to let them know new data is available.
		pthread_cond_signal(&condc);

		// Leave region with exclusive access.
		pthread_mutex_unlock(&mutex);
		i++;
	}

	return 0;
}

void *Consumer(void *arg) {

	int i=0,j;
	int id = *(int*)arg;      //caster om argument til int

	while(i < NUM_ITER) {
		// Pretend to use the data for something, by doing a random wait.
		usleep(rand()%MAX_MOD);

		// Enter critical region, with exclusive access.
		pthread_mutex_lock(&mutex);

		// While the buffer is empty, wait on the contidionvariable
		while(g_idx==0)
			pthread_cond_wait(&condc,&mutex);

		// Remove some data from the buffer
		g_data[g_idx-1]=0;
		g_idx--;

		// Print the status of the buffer         //lagt til id for consumer
		j=0; printf("(Consumer %d, buffer index is %d)\t", id, g_idx);
		while(j < g_idx) { j++; printf("="); } printf("\n");

		// Signal threads waiting on the producer conditionvariable
		// that a new free slot is available
		pthread_cond_signal(&condp);

		// Leave critical region
		pthread_mutex_unlock(&mutex);
		i++;
	}

	return 0;

}

