#include <stdio.h>
#include <conio.h>
#include <limits.h>
#define MAX 100

void roundRobin(int id[], int at[], int bt[], int n, int tq)
{
    int remaining_bt[MAX];
    int ct[MAX], tat[MAX], wt[MAX];
    int completed = 0, curt = 0;
    int queue[MAX];
    int front = 0, rear = 0;
    int inQueue[MAX] = {0};

    for (int i = 0; i < n; i++)
        remaining_bt[i] = bt[i];

    int minat = 999, first = 0;
    for (int i = 0; i < n; i++)
    {
        if (at[i] < minat)
        {
            minat = at[i];
            first = i;
        }
    }

    curt = at[first];
    queue[rear++] = first;
    inQueue[first] = 1;

    while (completed < n)
    {
        if (front == rear)
        {
            curt++;

            for (int i = 0; i < n; i++)
            {
                if (!inQueue[i] && at[i] <= curt && remaining_bt[i] > 0)
                {
                    queue[rear++] = i;
                    inQueue[i] = 1;
                }
            }
            continue;
        }

        int i = queue[front++];
        int exec_time = (remaining_bt[i] > tq) ? tq : remaining_bt[i];

        curt += exec_time;
        remaining_bt[i] -= exec_time;

        for (int j = 0; j < n; j++)
        {
            if (!inQueue[j] && at[j] <= curt && remaining_bt[j] > 0)
            {
                queue[rear++] = j;
                inQueue[j] = 1;
            }
        }

        if (remaining_bt[i] > 0)
        {
            queue[rear++] = i;
        }
        else
        {
            ct[i] = curt;
            completed++;
        }
    }

    for (int i = 0; i < n; i++)
    {
        tat[i] = ct[i] - at[i];
        wt[i] = tat[i] - bt[i];
    }

    // Display results
    printf("\n\nID\tAT\tBT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++)
    {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n", id[i], at[i], bt[i], ct[i], tat[i], wt[i]);
    }
}

void sjfp(int id[], int at[], int bt[], int n)
{
    int remaining_bt[n];
    int ct[n], tat[n], wt[n];
    int completed = 0, curt = 0;

    // Copy burst times
    for (int i = 0; i < n; i++)
        remaining_bt[i] = bt[i];

    while (completed != n)
    {
        int minIndex = -1;
        int minBT = INT_MAX;

        // Find process with minimum remaining time among arrived
        for (int i = 0; i < n; i++)
        {
            if (at[i] <= curt && remaining_bt[i] > 0 && remaining_bt[i] < minBT)
            {
                minBT = remaining_bt[i];
                minIndex = i;
            }
        }

        // If no process has arrived yet
        if (minIndex == -1)
        {
            curt++;
            continue;
        }

        // Execute process for 1 unit
        remaining_bt[minIndex]--;
        curt++;

        // If process completes
        if (remaining_bt[minIndex] == 0)
        {
            completed++;
            ct[minIndex] = curt;
        }
    }

    // Calculate TAT and WT
    for (int i = 0; i < n; i++)
    {
        tat[i] = ct[i] - at[i];
        wt[i] = tat[i] - bt[i];
    }

    // Display
    printf("\nID\tAT\tBT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++)
    {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n", id[i], at[i], bt[i], ct[i], tat[i], wt[i]);
    }
}

void sjfnp(int id[], int at[], int bt[], int n)
{
    int tt = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n - 1 - i; j++)
        {
            if (at[j] > at[j + 1])
            {
                int temp1 = at[j];
                at[j] = at[j + 1];
                at[j + 1] = temp1;

                int temp2 = bt[j];
                bt[j] = bt[j + 1];
                bt[j + 1] = temp2;

                int temp3 = id[j];
                id[j] = id[j + 1];
                id[j + 1] = temp3;
            }
        }
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n - 1 - i; j++)
        {
            if (at[j] == at[j + 1] && bt[j] > bt[j + 1])
            {
                int temp1 = at[j];
                at[j] = at[j + 1];
                at[j + 1] = temp1;

                int temp2 = bt[j];
                bt[j] = bt[j + 1];
                bt[j + 1] = temp2;

                int temp3 = id[j];
                id[j] = id[j + 1];
                id[j + 1] = temp3;
            }
        }
    }

    int ct[n];
    int tat[n];
    int wt[n];

    for (int i = 0; i < n; i++)
    {
        if (at[i] > tt)
        {
            tt = at[i];
        }
        tt += bt[i];
        ct[i] = tt;
        tat[i] = ct[i] - at[i];
        wt[i] = tat[i] - bt[i];
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n - 1 - i; j++)
        {
            if (id[j] > id[j + 1])
            {
                int temp1 = id[j];
                id[j] = id[j + 1];
                id[j + 1] = temp1;

                int temp2 = bt[j];
                bt[j] = bt[j + 1];
                bt[j + 1] = temp2;

                int temp3 = at[j];
                at[j] = at[j + 1];
                at[j + 1] = temp3;

                int temp4 = ct[j];
                ct[j] = ct[j + 1];
                ct[j + 1] = temp4;

                int temp5 = tat[j];
                tat[j] = tat[j + 1];
                tat[j + 1] = temp5;

                int temp6 = wt[j];
                wt[j] = wt[j + 1];
                wt[j + 1] = temp6;
            }
        }
    }

    for (int i = 0; i < n; i++)
    {
        printf("ID: %d ; AT: %d ; BT : %d ; CT : %d ; TAT : %d ; WT : %d \n", id[i], at[i], bt[i], ct[i], tat[i], wt[i]);
    }
}

void fcfs(int id[], int at[], int bt[], int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n - 1 - i; j++)
        {
            if (at[j] > at[j + 1])
            {
                int temp1 = at[j];
                at[j] = at[j + 1];
                at[j + 1] = temp1;

                int temp2 = bt[j];
                bt[j] = bt[j + 1];
                bt[j + 1] = temp2;

                int temp3 = id[j];
                id[j] = id[j + 1];
                id[j + 1] = temp3;
            }
        }
    }
    int ct[n];
    int tat[n];
    int wt[n];
    int tt = 0;
    for (int i = 0; i < n; i++)
    {
        if (at[i] > tt)
        {
            tt = at[i];
        }
        tt += bt[i];
        ct[i] = tt;
        tat[i] = ct[i] - at[i];
        wt[i] = tat[i] - bt[i];
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n - 1 - i; j++)
        {
            if (id[j] > id[j + 1])
            {
                int temp1 = id[j];
                id[j] = id[j + 1];
                id[j + 1] = temp1;

                int temp2 = bt[j];
                bt[j] = bt[j + 1];
                bt[j + 1] = temp2;

                int temp3 = at[j];
                at[j] = at[j + 1];
                at[j + 1] = temp3;

                int temp4 = ct[j];
                ct[j] = ct[j + 1];
                ct[j + 1] = temp4;

                int temp5 = tat[j];
                tat[j] = tat[j + 1];
                tat[j + 1] = temp5;

                int temp6 = wt[j];
                wt[j] = wt[j + 1];
                wt[j + 1] = temp6;
            }
        }
    }

    for (int i = 0; i < n; i++)
    {
        printf("ID: %d ; AT: %d ; BT : %d ; CT : %d ; TAT : %d ; WT : %d \n", id[i], at[i], bt[i], ct[i], tat[i], wt[i]);
    }
}

int main()
{

    int ch;

    do
    {
        printf("========== CPU SCHEDULING ==========");
        printf("\n1.FCFS\n2.SJF(Non-Preemptive)\n3.SJF(Preemptive)\n4.Round-Robin\n5.Exit\n");
        printf("\nEnter your choice : ");
        scanf("%d", &ch);

        if(ch == 5){
            printf("Exited !!");
            continue;
        }

        int n;
        printf("How many processes do you have to add: ");
        scanf("%d", &n);
        int at[n];
        int bt[n];
        int id[n];
        for (int i = 0; i < n; i++)
        {
            id[i] = i + 1;
        }
        printf("Enter space separated Arrival times of all %d processes: ", n);
        for (int i = 0; i < n; i++)
        {
            scanf("%d", &at[i]);
        }

        printf("Enter space separated Burst times of all %d processes: ", n);
        for (int i = 0; i < n; i++)
        {
            scanf("%d", &bt[i]);
        }

        int tq;
        if(ch == 4){
            printf("Enter time quantum fro Round Robin: ");
            scanf("%d",&tq);
        }

        switch(ch){
            case 1: 
                fcfs(id,at,bt,n);
                break;
            case 2: 
                sjfnp(id,at,bt,n);
                break;
            case 3:
                sjfp(id,at,bt,n);
                break;
            case 4:
                roundRobin(id, at, bt, n, tq);
                break;
            default:
                printf("Invalid choice !!");
        }



    } while (ch != 5);

        

    return 0;
}