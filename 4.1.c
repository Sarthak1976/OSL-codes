#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
#include<stdlib.h>


#define max 100

int buffer[max];
int in = 0;
int out = 0;

int n;
int items;
int *input; 

sem_t full,empty;
pthread_mutex_t mutex;

void* producer(void* arg){
    for(int i = 0 ; i < items ; i++){
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        buffer[in] = input[i];
        print("Producer: puts %d at %d .",input[i],in);
        in = (in + 1) % n ;

        pthread_mutex_unlock(&mutex);
        sem_post(&full);

    }
    return NULL;
}


void* consumer(void* arg){
    for(int i = 0; i < items;i++){
        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        int consumption = buffer[out];
        printf("Consumer: takes %d from %d .",buffer[out],out);
        out = (out + 1) % n;

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);

    }

    return NULL;
}


int main(){
    printf("Enter the Buffer Size: ");
    scnaf("%d",&n);
    printf("Enter number of items you want to produce/consume: ");
    scanf("%d",&items);

    input = malloc(items * sizeof(int));
    printf("Enter %d integers that you want to produce/consume : ",items);

    for(int i = 0;i < items;i++){
        scanf("%d",&input[i]);
    }

    pthread_t producerthread,consumerthread;

    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0 , n);
    sem_init(&full,0,0);

    pthread_create(&producerthread,NULL,producer,NULL);
    pthread_create(&consumerthread,Null,consumer,NULL);

    pthread_join(producerthread,  NULL);
    pthread_join(consumerthread, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;



}
