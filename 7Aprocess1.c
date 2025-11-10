#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>

#define FIFO1 "fifo1"
#define FIFO2 "fifo2"

int main(){
    int fd1, fd2;
    char sentence[100];
    char all_sentences[500] = "";
    char result_buffer[500];
    int i;

    mkfifo(FIFO1, 0666);
    mkfifo(FIFO2, 0666);

    printf("Process 1: Waiting for Process 2 to start...\n");

    fd1 = open(FIFO1, O_WRONLY);
    if(fd1 == -1){
        perror("open(FIFO1) error");
        exit(1);
    }

    fd2 = open(FIFO2, O_RDONLY);
    if(fd2 == -1){
        perror("open(FIFO2) error");
        exit(1);
    }


    printf("Process 1: Both processes are connected.\n\n");

    for(int i = 0;i < 3;i++){
        printf("Enter sentence %d: ",i+1);
        fgets(sentence,100,stdin);
        strcat(all_sentences, sentence);
    }

    write(fd1, all_sentences,strlen(all_sentences)+1);
    printf("\nProcess 1: Sent 3 sentences to Process 2.\n");

    read(fd2,result_buffer,sizeof(result_buffer));
    printf("Process 1: Received analysis from Process 2:\n----\n%s---\n",result_buffer);

    close(fd1);
    close(fd2);
    return 0;

}



