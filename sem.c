#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#include <string.h>
#include <unistd.h>
#include <fcntl.h>

/*union semun {
	int 			val;
	struct semid_ds *buf;
	unsigned short	*array;
	struct seminfo	*__buf;
};*/

int main(int argc, char *argv[]) {
	if (strcmp(argv[1],"-c") == 0) {
		int fd = open("story.txt",O_CREAT | O_TRUNC, 0644);
		
		int shmid = shmget(ftok("story.txt",21), 1024, IPC_CREAT | 0644);
		char * story = shmat(shmid, 0, 0);
	
		int semid = semget(ftok("story.txt",42), 1, IPC_CREAT | 0644);
		union semun su;
		su.val = 1;
		semctl(semid,0,SETVAL,su);
	} else if (strcmp(argv[1],"-v") == 0) {
		int shmid = shmget(ftok("story.txt",21), 1024, IPC_CREAT | 0644);
		
		char * story = shmat(shmid, 0, 0);
		int fd = open("story.txt",O_RDONLY, 0644);
		read(fd,story,1024);
		printf("%s\n",story);
		
		shmdt(story);
	} else if (strcmp(argv[1],"-r") == 0) {
		int shmid = shmget(ftok("story.txt",21), 1024, IPC_CREAT | 0644);
		
		char * story = shmat(shmid, 0, 0);
		int fd = open("story.txt",O_RDONLY, 0644);
		read(fd,story,1024);
		printf("%s\n",story);
		
		shmdt(story);
		shmctl(shmid,IPC_RMID,0);
		
		int semid = semget(ftok("story.txt",42), 1, IPC_CREAT | 0644);
		semctl(semid,0,IPC_RMID,0);
	}
	
	return 0;
}