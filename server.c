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
#define NUM_KEYS 10;
#define MEMSIZE 8;
#define NUM_THREADS 3;

void provide(void *memory_segment, int clientnum);
void* componentsFactory();

int components = 0;
static sem_t sync_sem;
char signal[10] = {0}; 

/*공유 메모리 관리용 리스트 노드*/
typedef struct memory_segment {
    void* memory_segment;
    struct memory_segment *next;
} mem_seg;

void main() {
    int shmid[10];
    int initvalue = -1;
    int clientnum=0;
    int memsize = MEMSIZE;
    key_t keys[] = {(key_t)60110, (key_t)60111, (key_t)60112, (key_t)60113, (key_t)60114, (key_t)60115, (key_t)60116, (key_t)60117, (key_t)60118, (key_t)60119}; // 이거 외 상수로 하믄 않됨?
    pthread_t threads[3];
    int rc;
    void *memory_segment=NULL;
    sem_init(&sync_sem, 0, 1);

    for(int i = 0 ; i < 10 ; i++) {
        if((shmid[i]=shmget(keys[i],sizeof(int),IPC_CREAT|0666))==-1) {
            printf("shmget faild. (point 1)\n");
        }
        if((memory_segment=shmat(shmid[i],NULL,0))==(void*)-1) {    // shared moemory 포인터를 바꿔가면서 검사
            printf("shmat failed. (point 2)\n");
            exit(0);
        }
        memcpy((int*)memory_segment, &initvalue, memsize);
    }

    for(int i = 0 ; i < 3 ; i++) {
        printf("In main : creating thread %d\n", i);
        rc = pthread_create(&threads[i], NULL, componentsFactory, NULL);
        if(rc) {
            printf("[ERROR] : return code from pthread_create is %d\n", rc);
            exit(-1);
        }
    }

    while(1) {
        if((memory_segment=shmat(shmid[clientnum],NULL,0))==(void*)-1) {    // shared moemory 포인터를 바꿔가면서 검사
            printf("shmat failed. (point 3, clientnum : %d)\n", clientnum);
            exit(0);
        }
        provide(memory_segment, clientnum);
        clientnum = ++clientnum % 10;
    }

}

void provide(void *memory_segment, int clientnum) {
    printf("Rotate num : %d\n", clientnum);
    int buffer = *(int*)memory_segment;
    int memsize = MEMSIZE;
    if(components > 0 && buffer < 5 && buffer != -1) {
        sem_wait(&sync_sem);
        components--;
        sem_post(&sync_sem);
        buffer++;
        memcpy((int*)memory_segment, &buffer, memsize);
        buffer--;
    }
    sleep(1);
}

void* componentsFactory() {
    srand(time(NULL));
    int productivity = rand()%4+5;

    while(1){
        if(components < 20) {
            sem_wait(&sync_sem);
            components++;
            sem_post(&sync_sem);
            printf("Thread : Component Supplied, (Quentity : %d)\n", components);
        }

        sleep(productivity);
    }
}