#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <pthread.h>

#define N 4

long int compteur=0;
pthread_mutex_t verrou;

void * fct(void * arg)
{
	int i;
 	for(i=0; i<1000000;++i)
 	{
 		pthread_mutex_lock(&verrou);
 		compteur++;
 		pthread_mutex_unlock(&verrou);
 	}
 	return NULL;
}

int main(int argc, char * argv[])
{
	pthread_t tid[N];
	int i, err;
	int tpar[N];

	if(pthread_mutex_init(&verrou, NULL) != 0)
	{
		perror("pthread_mutex_init()");
		exit(1);
	}

	for(i=0;i<N;++i)
	{
		tpar[i]=i;
		err=pthread_create(&tid[i], NULL, fct, &tpar[i]);
		if (err < 0) 
		{
			perror("pthread_create()");
			exit(1);
		}
	}
	for(i=0; i<N; ++i)
	{
		err=pthread_join(tid[i], NULL);
		if (err < 0)
		{
			perror("pthread_join()");
			exit(1);
		}
	}
	pthread_mutex_destroy(&verrou);
	printf("%ld \n", compteur);
	return 0;
}