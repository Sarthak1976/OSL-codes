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


    shmid = shmget(SHM_KEY,sizeof(struct shared_buffer),0666 | IPC_CREAT);

    if(shmid == -1){
        perror("shmget failed");
        exit(1);
    }
    printf("Server: Shared memory segment created(ID:%d)\n",shmid);

    shared_memory = shmat(shmid,NULL,0);
    if(shared_memory == (void*)-1){
        perror("shmat failed");
        exit(1);
    }
    printf("Server: Shared memory attached.\n");

    shm_ptr = (struct shared_buffer*)shared_memory;

    shm_ptr->server_ready = 0;
    shm_ptr->client_ready = 0;

    printf("\nEnter a message to send to the client: ");
    fgets(shm_ptr->message,200,stdin);

    printf("Server: Message written.Flag set.Waiting for client...\n");
    shm_ptr->server_ready = 1;

    while(shm_ptr->client_ready==0){
        sleep(1);
    }
    printf("Server: Client has read the message.\n");

    if(shmdt(shared_memory)== -1){
        perror("shmdt failed");
        exit(1);
    }

    if(shmctl(shmid,IPC_RMID,NULL)==-1){
        perror("shmctl(IPC_RMID) failed");
        exit(1);
    }

    printf("Server: Shared memory Detached and removed .Exiting.\n");

    return 0;

}
