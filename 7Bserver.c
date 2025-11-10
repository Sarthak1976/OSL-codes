#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<unistd.h>


#define SHM_KEY 6168

struct shared_buffer{
    int server_ready;
    int client_ready;
    char message[200]; 
};

int main(){
    int shmid;
    void *shared_memory;
    struct shared_buffer *shm_ptr;
    
    shmid = shmget(SHM_KEY,sizeof(struct shared_buffer),0666);
    if(shmid == -1){
        perror("shmget failed");
        exit(1);
    }
    printf("Client: Found shared momory segment(ID: %d)\n",shmid);

    shared_memory = shmat(shmid,NULL,0);
    if(shared_memory==(void *)-1){
        perror("shmat failed");
        exit(1);
    }
    printf("Client: Shared memory attached.\n");

    shm_ptr = (struct shared_buffer *)shared_memory;

    printf("Client: Waiting for server to write..\n");
    while(shm_ptr->server_ready==0){
        sleep(1);
    }

    printf("\n--- Client Read Message ---\n");
    printf("%s",shm_ptr->message);
    printf("------------------------\n");

    shm_ptr->client_ready=1;
    printf("Client: Set clien_ready flag. Detaching.\n");

    if(shmdt(shared_memory)==-1){
        perror("shmdt failed");
        exit(1);
    }

    printf("Client: Exiting.\n");
    return 0;
}
