#include<stdio.h>
int main(){
    int   p,r;
    printf("\nEnter number of processes: ");
    scanf("%d",&p);

    printf("\nEnter number of resources: ");
    scanf("%d",&r);

    int alloc[p][r];
    int max[p][r];
    int available[r];
    int need[p][r];
    int finish[p];
    int work[r];
    int safeSequence[p];

    printf("\nEnter the available resources: ");
    for(int i = 0; i < r;i++){
        scanf("%d",&available[i]);
    }

    printf("\nEnter the allocation matrix row wise: \n");
    for(int i = 0;i < p;i++){
        printf("P%d: ",i+1);
        for(int j = 0;j < r;j++){
            scanf("%d",&alloc[i][j]);
        }
    }

    printf("\nEnter the Max matrix row wise: \n");
    for(int i = 0;i < p;i++){
        printf("P%d: ",i+1);
        for(int j = 0;j < r;j++){
            scanf("%d",&max[i][j]);
        }
    }

    for(int i = 0;i <p;i++){
        for(int j = 0;j < r;j++){
            need[i][j] = max[i][j] - alloc[i][j];
        }
    }

    for(int i = 0; i<p;i++){
        finish[i] = 0;
    }

    for(int i =0 ;i < r;i++){
        work[i] = available[i];
    }


    int count = 0;
    int found;

    for(int pass = 0;pass < p;pass++){
        found = 0;

        for(int i = 0;i < p;i++){
            if(finish[i] == 0){
                int canAllocate = 1;
                for(int j = 0;j<r;j++){
                    if(need[i][j] > work[j]){
                        canAllocate = 0;
                        break;
                    }
                }

                if(canAllocate == 1){
                    printf("Process P%d can run. \n",i);
                    printf("Releasing its resources. New work = \n");
                    for(int j =0 ;j < r;j++){
                        work[j] += alloc[i][j];
                        printf("%d ",work[j]);
                    }

                    printf("\n");
                    finish[i] = 1;

                    safeSequence[count] = i+1;
                    count++;

                    found = 1;

                }
            }
        }

        if(found == 0){
            break;
        }
    }


    if(count == p){
        printf("\nSysem is in safe state.\n");
        printf("The safe sequence is: ");
        for(int i = 0;i <p;i++){
            printf("P%d",safeSequence[i]);
            if(i < p-1){
                printf(" -> ");
            }
        }
        printf("\n");
    }else{
        printf("\nSystem is in an UNSAFE STATE.\n");
        printf("The algorithm got stuck.\n");
        printf("Processes that could not finish: ");
        for (int i = 0; i < p; i++) {
            if (finish[i] == 0) {
                printf("P%d ", i);
            }
        }
        printf("\n");
    }







    return 0;
}