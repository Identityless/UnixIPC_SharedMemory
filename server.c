#include <stdio.h>
#include <sys/shm.h> 
#include <sys/ipc.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>

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

void provide(void *memory_segment);

int components = 0;
static sem_t sync_sem;
char signal[10] = {0}; 

typedef struct memory_segment {
    void* memory_segment;
    struct memory_segment *next;
} mem_seg;

void main() {
    int shmid;
    mem_seg *head;
    key_t key = SHMKEY0;
    void *memory_segment=NULL;
    sem_init(&sync_sem, 0, 1);

    if((shmid=shmget(key,sizeof(int),IPC_CREAT|0666))==-1) {
        printf("shmget faild\n");
    }

    if((memory_segment=shmat(shmid,NULL,0))==(void*)-1){
        printf("shmat failed\n");
        exit(0);
    }

    while(1) {
        provide(memory_segment);
    }

    if((memory_segment=shmat(shmid,NULL,0))==(void*)-1){
            printf("shmat failed\n");
            exit(0);
    }

}

void provide(void *memory_segment) {
    int buffer = 0;
    int memsize = MEMSIZE;
    if(components > 0) {
        sem_trywait(&sync_sem);
        components--;
        sem_post(&sync_sem);
        buffer++;
        memcpy((int*)memory_segment, &buffer, memsize);
        buffer--;
    }
}

void* componentsFactory() {
    srand(time(NULL));
    int productivity = rand()%4+5;

    if(components < 20) {
        sem_wait(&sync_sem);
        components++;
        sem_post(&sync_sem);
    }

    sleep(productivity);
}