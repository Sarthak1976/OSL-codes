#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<ctype.h>

#define FIFO1 "fifo1"
#define FIFO2 "fifo2"

int count_words(const char *str){
    int count = 0;
    int in_word = 0;
    while(*str){
        if(isspace(*str)){
            in_word = 0;
        }else if(in_word == 0){
            in_word = 1;
            count++;
        }
        str++;
    }
    return count;
}

int main(){
    int fd1, fd2;
    char received_data[500];
    char file_content[500];
    FILE *fp;

    int char_count = 0;
    int word_count = 0;
    int line_count = 3;

    mkfifo(FIFO1, 0666);
    mkfifo(FIFO2, 0666);

    printf("Process 2: Waiting for Process 1 to send data...\n");

    fd1 = open(FIFO1, O_RDONLY);
    if(fd1 == -1){
        perror("open(FIFO1) error");
        exit(1);
    }

    fd2 = open(FIFO2, O_WRONLY);
    if(fd2 == -1){
        perror("open(FIFO2) error");
        exit(1);
    }

    printf("Prrocess 2: Connected.\n ");

    read(fd1,received_data,sizeof(received_data));
    printf("Process 2: Received data:\n%s\n",received_data);

    char_count = strlen(received_data)-1;
    word_count = count_words(received_data);

    fp = fopen("output.txt","w");
    if(fp == NULL){
        perror("fopen(output.txt) error");
        exit(1);
    }

    fprintf(fp, "Analysis Report:\n");
    fprintf(fp, "Character Count: %d\n",char_count);
    fprintf(fp, "Word Count: %d\n",word_count);
    fprintf(fp, "Line Count: %d\n",line_count);
    fclose(fp);
    printf("Process 2: Analysis saved to output.txt.\n");

    fp = fopen("output.txt","r");
    if(fp==NULL){
        perror("fopen(output.txt) read error");
        exit(1);
    }

    fread(file_content,1,sizeof(file_content)-1,fp);
    file_content[strlen(file_content)] = '\0';
    fclose(fp);

    write(fd2,file_content,strlen(file_content)+1);
    printf("Process 2: Sent analysis back to Process 1.\n");

    close(fd1);
    close(fd2);

    unlink(FIFO1);
    unlink(FIFO2);
    return 0;
}
