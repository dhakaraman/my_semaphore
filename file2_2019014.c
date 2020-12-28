//Aman
//2019014

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define n 5


struct my_semaphore {
	pthread_mutex_t mutex;
	int val;
}room,bowls[2],chopstick[n];	


void my_sem_init(struct my_semaphore *s, int value) {
	pthread_mutex_init(&s->mutex, NULL);
	s->val = value;
}

int wait(struct my_semaphore *s) {
	pthread_mutex_lock(&s->mutex);
	s->val--;
	if(s->val < 0) {
		s->val++;
		pthread_mutex_unlock(&s->mutex);
		return -1;
	}
	pthread_mutex_unlock(&s->mutex);
	return 0;
}

void signal(struct my_semaphore *s) {
	pthread_mutex_lock(&s->mutex);
	s->val++;
 	pthread_mutex_unlock(&s->mutex);
}

int _signal(struct my_semaphore *s) { //signal(printValue)
	return s->val;
}


void * philosopher(int phil)
{	

while(1){
	
		while(wait(&room) == -1);
		
		printf("P%d has entered in room %d\n", phil, phil);
		
		while(wait(&chopstick[phil]) == -1);
		while(wait(&chopstick[(phil+1)%n]) == -1);
		
		printf("P%d has received fork %d and fork %d\n",phil,phil, (phil+1)%n);
		
		while(wait(&bowls[0]) == -1);
		while(wait(&bowls[1]) == -1);
		
		printf("P%d received bowl %d bowl %d and is eating.\n", phil,0,1);
		
		sleep(1);
		signal(&bowls[1]);
		signal(&bowls[0]);
		signal(&chopstick[(phil+1)%n]);
		signal(&chopstick[phil]);
		signal(&room);
	
	}
}

int main()
{
	int i;
	pthread_t tid[n];
	my_sem_init(&room,n-1);
	my_sem_init(&bowls[0],1);
	my_sem_init(&bowls[1],1);
	for(i=0;i<n;i++) {
		my_sem_init(&chopstick[i],1);
	}
	for(i=0;i<n;i++){
		pthread_create(&tid[i],NULL,philosopher,i);
	}
	for(i=0;i<n;i++)
		pthread_join(tid[i],NULL);
}

