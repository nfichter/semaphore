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
	
	int semid = semget(ftok("story.txt",42),1,0644);
	
	struct sembuf sb;
	sb.sem_num = 0;
	sb.sem_flg = SEM_UNDO;
	sb.sem_op = -1;
	semop(semid,&sb,1);
	
	int shmid = shmget(ftok("story.txt",21), 1024, IPC_CREAT | 0644);
	int fd = open("story.txt",O_RDWR | O_APPEND, 0644);

	/*
	//read last line
	//seg faults from this part
	int *mem_seg = (int *) shmat(shmid, 0, 0);
	char last_line[1000];
	lseek(fd,-(*mem_seg),SEEK_END);
	read(fd,last_line,*mem_seg);

  	if(strlen(last_line) != 0) printf("Last line: %s\n",last_line);
	*/

	printf("Add a line to the story:\n");
	
	char buffer[64];
  	fgets(buffer, sizeof(buffer), stdin);
  	int messageLen = strlen(buffer);
  	printf("messageLen: %d\n message: %s",messageLen,buffer);
  	write(fd,buffer,messageLen);
	
	sb.sem_op = 1;
	semop(semid,&sb,1);
	//shmdt(story);

	return 0;
}