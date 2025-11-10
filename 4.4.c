#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>

int available_flights = 100; //Shared variable

sem_t db_access;
pthread_mutex_t mutex;
int reader_count = 0;

void* reader_thread(void* arg){
    int id = *(int*)arg;
    printf("\nReader %d: Arrived, wants to check flights.\n",id);

    pthread_mutex_lock(&mutex);
    reader_count++;
    printf("\nReaders in the system: %d \n",reader_count);
    if(reader_count==1){
        printf("\nReader %d: I am the first reader.Locking database for writers.\n",id);
        sem_wait(&db_access);
    }

    pthread_mutex_unlock(&mutex);

    printf("\nReader %d: Reading flight info...(%d flights availabel)\n",id,available_flights);
    sleep(1);
    printf("\nReader %d: Finished reading.\n",id);

    pthread_mutex_lock(&mutex);
    reader_count--;
    printf("\nReaders in the system:%d\n ",reader_count);
    if(reader_count==0){
        printf("\nReader %d: I am the last reader.Unlocking database for writers.\n",id);
        sem_post(&db_access);
    }
    pthread_mutex_unlock(&mutex);

    printf("\nReader %d: Exited the system.\n",id);

    pthread_exit(NULL);
}


void* writer_thread(void* arg){
    int id = *(int*)arg;

    printf("\nWriter %d: Arrived, wants to book a flight.\n",id);

    printf("\nWriter %d: Waiting for database access...\n",id);
    sem_wait(&db_access);

    printf("\nWrite %d: writing to database...\n",id);
    available_flights--;
    sleep(1);
    printf("\nWriter %d: Done writing,(%d flights left)\n",id,available_flights);

    sem_post(&db_access);

    printf("\nWriter %d: Exited the system.\n",id);
    printf("\nReaders in the system: %d \n",reader_count);

    pthread_exit(NULL);
}

#define num_of_readers 5
#define num_of_writers 2

int main(){
    pthread_t readers[num_of_readers],writers[num_of_writers];
    int reader_ids[num_of_readers],writer_ids[num_of_writers];

    pthread_mutex_init(&mutex,NULL);

    sem_init(&db_access,0,1);

    for(int i = 0;i < num_of_readers;i++){
        reader_ids[i] = i+1;
        pthread_create(&readers[i],NULL,reader_thread,&reader_ids[i]);
    }

    for(int i = 0;i < num_of_writers;i++){
        writer_ids[i] = i+1;
        pthread_create(&writers[i],NULL,writer_thread,&writer_ids[i]);
    }

    for(int i = 0;i < num_of_readers;i++){
        pthread_join(readers[i],NULL);
    }

    for(int i = 0;i < num_of_writers;i++){
        pthread_join(writers[i],NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&db_access);

    printf("\nMain thread: All flights processed.Final count: %d\n",available_flights);

    return 0;
}
