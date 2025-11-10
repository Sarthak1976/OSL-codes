#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>

#define num_of_chairs 3
#define num_of_students 10

sem_t students;
sem_t ta_ready;
pthread_mutex_t mutex;

int waiting_count = 0;

void* ta_thread(void* arg){
    while(1){
        sem_wait(&students);

        pthread_mutex_lock(&mutex);

        waiting_count--;
        printf("\nTA : Helping a student. Student waiting: %d\n",waiting_count);

        sem_post(&ta_ready);

        pthread_mutex_unlock(&mutex);

        printf("\nTA: ...giving help...\n");
        sleep(2);
        printf("\nTA : Finished helping. Looking for next student\n")

    }
    return NULL;
}

void* student_thread(void* arg){
    int id = *(int*)arg;

    pthread_mutex_lock(&mutex);

    if(waiting_count < num_of_chairs){
        waiting_count++;
        printf("\nStudent %d : Sitting down,Students Waiting: %d\n",id,waiting_count);

        sem_post(&students);

        pthread_mutex_unlock(&mutex);

        sem_wait(&ta_ready);

        printf("Student %d: Getting help from TA.\n",id);
    }else{
        printf("Student %d: No chairs free. Leaving.\n",id);

        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

int main(){
    pthread_t ta;
    pthread_t students_threads[num_of_students];
    int student_ids[num_of_students];

    pthread_mutex_init(&mutex,NULL);
    sem_init(&students,0,0);
    sem_init(&ta_ready,0,0);

    pthread_create(&ta,NULL,ta_thread,NULL);

    for(int i = 0;i < num_of_students;i++){
        student_ids[i] = i+1;
        pthread_creaate(&students_threads[i],NULL,student_thread,&student_ids[i]);
        sleep(1);
    }

    for(in ti = 0;i < num_of_students;i++){
        pthread_join(students_threads[i],NULL);
    }



    printf("\nAll students have finished.\n");

    return 0;

}