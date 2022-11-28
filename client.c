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

void checkUsableKey();
void choiceKey(key_t *key);

void main() {
    int msg;
    int memsize = MEMSIZE;
    int shmid;
    key_t key;
    void *memory_segment=NULL;

    checkUsableKey();
    choiceKey(&key);

    if((shmid=shmget(key,sizeof(int),IPC_CREAT|0666))==-1) {
        printf("shmget faild\n");
    }

    if((memory_segment=shmat(shmid,NULL,0))==(void*)-1){
        printf("shmat failed\n");
        exit(0);
    }

}

void choiceKey(key_t *key) {
    int buffer = 0;

    printf("사용할 키 입력 : ");
    scanf("%d", &buffer);

    *key = (key_t)buffer;
}

void checkUsableKey() {
    key_t keys[] = {(key_t)60110, (key_t)60111, (key_t)60112, (key_t)60113, (key_t)60114, (key_t)60115, (key_t)60116, (key_t)60117, (key_t)60118, (key_t)60119};
    int shmid[10];
    void *memory_segment=NULL;
    int readpoint;

    for(int i = 0 ; i < 10 ; i++) {
        if((shmid[i]=shmget(keys[i],sizeof(int),IPC_CREAT|0666))==-1) {
            printf("shmget faild. (point 1)\n");
        }
        if((memory_segment=shmat(shmid[i],NULL,0))==(void*)-1) {    // shared moemory 포인터를 바꿔가면서 검사
            printf("shmat failed. (point 2)\n");
            exit(0);
        }
        memcpy(&readpoint, (int*)memory_segment, sizeof(int));

        if(readpoint == -1)
        {
            printf("%d key - 사용 가능\n", (int)keys[i]);
        }
        else {
            printf("%d key - 사용 불가\n", (int)keys[i]);
        }
    }
}