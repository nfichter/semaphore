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

  int shmid = shmget(ftok("makefile",21), 1024, 0644);
  int fd = open("story.txt",O_RDWR | O_APPEND, 0644);

  if (shmid == -1) {
    printf("Please create using ./sem -c before running the control file.\n");
  } else {
    int *mem_seg = (int *)shmat(shmid, 0, 0);
    char * last_line = (char *)calloc(sizeof(char),1024);
    lseek(fd,-(*mem_seg),SEEK_END);
    read(fd,last_line,*mem_seg);
	
    if (*mem_seg != 0) {
      printf("Last line: %s",last_line);
    }
	
    printf("Add a line to the story:\n");
	  
    char buffer[64];
    fgets(buffer, sizeof(buffer), stdin);
    char * zeroPoint = strstr(buffer,"\n");
    zeroPoint = 0;
    int messageLen = strlen(buffer);
    write(fd,buffer,messageLen);
    
    *mem_seg = messageLen;
	
    sb.sem_op = 1;
    semop(semid,&sb,1);
	  
    return 0;
  }
}
