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
    int shmid = shmget(ftok("makefile",21), 1024, IPC_CREAT | IPC_EXCL | 0644);
    if (shmid == -1) {
      printf("Semaphore already exists, please remove using ./sem -r before re-creating.\n");
    } else {
      int fd = open("story.txt",O_CREAT | O_TRUNC, 0644);
      int semid = semget(ftok("makefile",42), 1, IPC_CREAT | IPC_EXCL | 0644);
      printf("Semaphore created: %d\n", semid);
      union semun su;
      su.val = 1;
      semctl(semid,0,SETVAL,su);
    }
  }
  else if (strcmp(argv[1],"-v") == 0) {
    int shmid = shmget(ftok("makefile",21), 1024, 0644);
    if (shmid == -1) {
      printf("Please create using ./sem -c before attempting to view the file.\n");
    } else {
      char * story = (char *)calloc(sizeof(char),1024);
      int fd = open("story.txt",O_RDONLY, 0644);
      read(fd,story,1024);
      printf("Viewing Story: %s\n",story);
    }
  } else if (strcmp(argv[1],"-r") == 0) {
    int shmid = shmget(ftok("makefile",21), 1024, 0);
    if (shmid == -1) {
      printf("Please create using ./sem -c before attempting to remove the file.\n");
    } else {
      int semid = semget(ftok("makefile",42), 1, 0);
      char * story = (char *)calloc(sizeof(char),1024);
      int fd = open("story.txt",O_RDONLY, 0644);
      read(fd,story,1024);
      printf("Removing story: %s\n",story); 
      shmctl(shmid,IPC_RMID,0);
      semctl(semid,0,IPC_RMID,0);
      remove("story.txt");
    }
  }
  return 0;
}
