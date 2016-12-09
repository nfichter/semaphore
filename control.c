#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include <time.h>

int main() {
	srand(time(NULL));
	int x = random() % 10 + 5;
	
	int semid = semget(ftok("story.txt",42),1,0);
	
	struct sembuf sb;
	sb.sem_num = 0;
	sb.sem_flg = SEM_UNDO;
	sb.sem_op = -1;
	
	semop(semid,&sb,1);
	
	int shmid = shmget(ftok("story.txt",21), 1024, IPC_CREAT | 0644);
	char * story = shmat(shmid, 0, 0);
	
	printf("Add a line to the story:\n");
	
	char * buffer = calloc(1024,sizeof(char));
  	fgets(buffer, 64, stdin);
  	
  	strcat(story,buffer);
  	int fd = open("story.txt",O_RDWR, 0644);
  	write(fd,story,1024);
	
	sb.sem_op = 1;
	semop(semid,&sb,1);
	shmdt(story);
	
	return 0;
}