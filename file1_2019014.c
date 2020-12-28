//Aman
//2019014



#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define n 5

struct my_semaphore {
	pthread_mutex_t mutex;  //mutex, for mutual exclusion
	pthread_cond_t cond;    //when we want to sleep or wait a thread 
	int val;
	int wait;
}room,chopstick[n],bowls;	


void my_sem_init(struct my_semaphore *s, int value) {
	s->val = value;
	pthread_mutex_init(&s->mutex, NULL);    //this function is used for initializing mutexes
	pthread_cond_init(&s->cond, NULL);      //initialises the condition variable
	s->wait = 0;
}

void wait(struct my_semaphore *s) {
	//printf("wait: %d",s->val);
	pthread_mutex_lock(&s->mutex);          //locking the mutex
	s->val--;
	if(s->val < 0) {
		//printf("if wait: %d",s->val);
		pthread_cond_wait(&s->cond, &s->mutex);
		while(s->wait < 1){
			pthread_cond_wait(&s->cond, &s->mutex);  //in suspeded state i.e. waiting
		} 
		s->wait--;									 //mutex is woken up
	}
	pthread_mutex_unlock(&s->mutex);                 //mutex unlocked and is woken up
}

void signal(struct my_semaphore *s) {
	
	//printf("value %d",s->val)
	pthread_mutex_lock(&s->mutex);					 
	s->val++;										 //incrementing value of semaphore i.e. taking out a semaphore from critcal section
	if (s->val <= 0) {
		pthread_cond_signal(&s->cond);				 //sending signal to all threads
		s->wait++;									
		//printf("I am here : %d",s->wait);
 	}
 	pthread_mutex_unlock(&s->mutex);
}

int _signal(struct my_semaphore *s) {
	return s->val;
}


void eat(int phil)
{
	printf("P%d has received bowl %d and bowl %d and is eating.\n", phil,0, 1);
	sleep(1);
}

void * philosopher(int phil)
{

	while(1){
			wait(&room);
			printf("P%d has entered in room %d\n", phil, phil);
			wait(&chopstick[phil]);
			printf("P%d has received fork %d and ", phil, phil);
			wait(&chopstick[(phil+1)%n]);
			printf("P%d has received fork %d and fork %d\n",phil,phil, (phil+1)%n);
			wait(&bowls);
			eat(phil);
			sleep(1);
			signal(&bowls);
			signal(&chopstick[(phil+1)%n]);
			signal(&chopstick[phil]);
			signal(&room);
		}
	
}

int main()
{
	pthread_t tid[n];
	
	my_sem_init(&room,n-1);
	my_sem_init(&bowls,1);
	for(int i=0;i<n;i++){ 
		my_sem_init(&chopstick[i],1);
	}
	for(int i=0;i<n;i++)
		pthread_create(&tid[i],NULL,philosopher,i);
	
	for(int i=0;i<n;i++)
		pthread_join(tid[i],NULL);
		
}

