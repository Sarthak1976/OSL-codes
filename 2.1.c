#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

void insertionSort(int arr[], int n) {
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;

        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

void bubbleSort(int arr[], int n){
    for(int i = 0;i < n;i++){
        for(int j = 0;j < n - 1 - i;j++){
            if(arr[j] > arr[j+1] ){
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}



int main(){
    int n;
    int i;
    printf("\nHow many numbers do you want to sort? \n");
    scanf("%d", &n);
    int a[n];
    printf("\nEnter the number with spaces that you want to sort: \n");
    for(i = 0;i < n;i++){
        scanf("%d", &a[i]);
    }
	
    
    pid_t ret = fork();

    if(ret < 0){
        printf("\nFork failed !\n");
    }else if(ret == 0){
        int j;
	int parentid = getppid();
        printf("\nChild Process with process ID %d started\n",getpid());
        printf("\nInsertion sort : \n");
        insertionSort(a,n);
	
        for(j = 0; j < n;j++){
            printf("%d ",a[j]);
        }
	//sleep(5); //uncomment this for orphan
	printf("\nChild process completed !\n");
	if(getppid() != parentid){
            printf("\nThere was a orphan process formation.\n");
	}
	//else{
	  //  printf("\nThere is a formation of zombie process\n");
	//} //uncomment this to demonstrate zombie process
	
    }else{
        printf("\nThis is Parent process with process ID %d started\n",getpid());
        printf("\nBubble sort: \n");
        bubbleSort(a,n);
        for(int j = 0; j < n;j++){
            printf("%d ",a[j]);
        }
	wait(NULL);
	//sleep(5); uncomment this to show zombie process
	
        printf("\nParent process finished !!\n");
	//printf("\nThere was a zombie process formation but it was removed from the process table\n"); //uncomment this when wait in uncommented
	
    }


    return 0;
}
