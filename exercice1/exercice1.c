#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

int main(int argc, char * argv[])
{
	int i, j, status;
	int pid[4];
	int shmid;
	int * shmadr;

	if ((shmid = shmget(IPC_PRIVATE, 
		sizeof(long int), IPC_CREAT|0600))==-1)
	{
		perror("shmget()");
		exit(1);
	}

	if ( (shmadr=shmat(shmid,NULL,0))==NULL)
	{
		perror("shmat()");
		exit(1);
	}

	for(i=0; i<4; i++)
	{
		if( (pid[i]=fork()) < 0)
		{
			perror("fork()");
			exit(1);
		}
		if (pid[i] == 0)
		{
			for(j=0; j<1000000; j++)
				shmadr[0]++;
			exit(0);
		}
	}

	for(i=0 ; i<4; i++)
	{
		if(wait(&status)==-1)
		{
			perror("wait()");
			exit(1);
		}
	}

	printf("%ld \n", (long int)shmadr[0]);
	if ( (shmctl(shmid, 0, IPC_RMID)) == -1 )
	{
		perror("shmctl()");
		exit(1);
	}
	return 0;
}

/* 
ipcs -m ( -m pour byte )
ipcrm shm 1212436

on supprime si shmctl est en commentire
il est présent en fin de programme, il supprime déjà 
les segments inutiles */

