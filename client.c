#include <stdio.h>
#include <sys/shm.h> 
#include <sys/ipc.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#define SHMKEY0 (key_t)60110;
#define SHMKEY1 (key_t)60111;
#define SHMKEY2 (key_t)60112;
#define SHMKEY3 (key_t)60113;
#define SHMKEY4 (key_t)60114;
#define SHMKEY5 (key_t)60115;
#define SHMKEY6 (key_t)60116;
#define SHMKEY7 (key_t)60117;
#define SHMKEY8 (key_t)60118;
#define SHMKEY9 (key_t)60119;
#define MEMSIZE 4;


void main() {
    int msg;
    int memsize = MEMSIZE;
    int shmid;
    key_t key = SHMKEY0;
    void *memory_segment=NULL;

    printf("데이터 입력 : ");
    scanf("%d", &msg);

    if((shmid=shmget(key,sizeof(int),IPC_CREAT|0666))==-1) {
        printf("shmget faild\n");
    }

    if((memory_segment=shmat(shmid,NULL,0))==(void*)-1){
        printf("shmat failed\n");
        exit(0);
    }

    memcpy(&num, (int*)memory_segment, memsize);
    printf("data : %d\n", num);


    if((memory_segment=shmat(shmid,NULL,0))==(void*)-1){
            printf("shmat failed\n");
            exit(0);
    }

}