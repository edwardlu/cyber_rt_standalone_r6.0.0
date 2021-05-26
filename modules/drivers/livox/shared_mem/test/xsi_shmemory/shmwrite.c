#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/shm.h>
#include "shmdata.h"
int main()
{  
    int running = 1;   
    void *shm = NULL;  
    struct shared_use_st *shared = NULL;
    char buffer[BUFSIZ + 1];
    int shmid;
    int shmid_test;

    shmid = shmget((key_t)12345678, sizeof(struct shared_use_st), 0644 | IPC_CREAT | IPC_EXCL);
    if(shmid == -1)
    {
	"shmget failed"<<std::endl;
    }

    shm = shmat(shmid, (void*)0, 0);
    if(shm == (void*)-1)
    {  
        fprintf(stderr, "shmat failed\n");     
        exit(EXIT_FAILURE);
    }  
    printf("Memory first attached at %X\n", (int)shm);
 
    shared = (struct shared_use_st*)shm;

    shmid_test = shmget((key_t)12345678, sizeof(struct shared_use_st), 0644 | IPC_CREAT | IPC_EXCL);
    if(shmid_test == -1)
    {
        fprintf(stderr, "shmget test failed\n");
        exit(EXIT_FAILURE);
    }
    shm = shmat(shmid_test, (void*)0, 0);
    if(shm == (void*)-1)
    {
        fprintf(stderr, "shmat test failed\n");
        exit(EXIT_FAILURE);
    }
    printf("Memory second attached at %X\n", (int)shm);

    while(running)
    {
        while(shared->written == 1)     
        {          
            sleep(1);      
            printf("Waiting...\n");
        }
        printf("Enter some text: ");
        fgets(buffer, BUFSIZ, stdin);
        strncpy(shared->text, buffer, TEXT_SZ);
        shared->written = 1;
        if(strncmp(buffer, "end", 3) == 0)
            running = 0;
    }

    if(shmdt(shm) == -1)
    {
        fprintf(stderr, "shmdt failed\n");
        exit(EXIT_FAILURE);
    }
    sleep(2);
    exit(EXIT_SUCCESS);
}
