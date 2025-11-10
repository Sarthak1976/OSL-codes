#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define CYLINDER_MAX 499
#define CYLINDER_MIN 0

int compare_int (const void *a,const void *b);
void print_array(int arr[], int size);
void algorithm_scan(int head, int requests[], int num_requests, int split_point_index);
void algorithm_look(int head, int requests[], int num_requests, int split_point_index);
void algorithm_cscan(int head, int requests[], int num_requests, int split_point_index);
void algorithm_clook(int head, int requests[], int num_requests, int split_point_index);

int compare_int (const void *a,const void *b){
    return (*(int  *)a - *(int *)b);
}
void print_array(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}


// 1. SCAN Algorithm
void algorithm_scan(int head, int requests[], int num_requests, int split_point_index) {
    int total_movement = 0;
    int i;
    
    printf("\n--- 1. SCAN Algorithm ---\n");
    printf("   Movement Path: %d", head);
    
    for (i = split_point_index; i < num_requests; i++) {
        printf(" -> %d", requests[i]);
    }
    printf(" -> %d", CYLINDER_MAX); // Go to the end
    
    for (i = split_point_index - 1; i >= 0; i--) {
        printf(" -> %d", requests[i]);
    }
    printf("\n");

    total_movement = (CYLINDER_MAX - head) + (CYLINDER_MAX - requests[0]);
    printf("   Total Head Movement: %d\n", total_movement);
    printf("   Average Seek Distance: %.2f\n", (float)total_movement / num_requests);
}

// 2. LOOK Algorithm
void algorithm_look(int head, int requests[], int num_requests, int split_point_index) {
    int total_movement = 0;
    int i;

    printf("\n--- 2. LOOK Algorithm ---\n");
    printf("   Movement Path: %d", head);

    for (i = split_point_index; i < num_requests; i++) {
        printf(" -> %d", requests[i]);
    }
    
    for (i = split_point_index - 1; i >= 0; i--) {
        printf(" -> %d", requests[i]);
    }
    printf("\n");
    
    int last_request = requests[num_requests - 1];
    total_movement = (last_request - head) + (last_request - requests[0]);
    printf("   Total Head Movement: %d\n", total_movement);
    printf("   Average Seek Distance: %.2f\n", (float)total_movement / num_requests);
}

// 3. C-SCAN Algorithm
void algorithm_cscan(int head, int requests[], int num_requests, int split_point_index) {
    int total_movement = 0;
    int i;
    
    printf("\n--- 3. C-SCAN Algorithm ---\n");
    printf("   Movement Path: %d", head);

    for (i = split_point_index; i < num_requests; i++) {
        printf(" -> %d", requests[i]);
    }
    printf(" -> %d", CYLINDER_MAX); 
    
    printf(" -> %d", CYLINDER_MIN); // Jump to other end
    for (i = 0; i < split_point_index; i++) {
        printf(" -> %d", requests[i]);
    }
    printf("\n");

    total_movement = (CYLINDER_MAX - head) + (CYLINDER_MAX - CYLINDER_MIN) + (requests[split_point_index - 1] - CYLINDER_MIN);
    printf("   Total Head Movement: %d\n", total_movement);
    printf("   Average Seek Distance: %.2f\n", (float)total_movement / num_requests);
}

// 4. C-LOOK Algorithm
void algorithm_clook(int head, int requests[], int num_requests, int split_point_index) {
    int total_movement = 0;
    int i;
    
    printf("\n--- 4. C-LOOK Algorithm ---\n");
    printf("   Movement Path: %d", head);
    
    for (i = split_point_index; i < num_requests; i++) {
        printf(" -> %d", requests[i]);
    }
    
    for (i = 0; i < split_point_index; i++) {
        printf(" -> %d", requests[i]);
    }
    printf("\n");

    total_movement = (requests[num_requests - 1] - head) + (requests[num_requests - 1] - requests[0]) + (requests[split_point_index - 1] - requests[0]);
    printf("   Total Head Movement: %d\n", total_movement);
    printf("   Average Seek Distance: %.2f\n", (float)total_movement / num_requests);
}


int main() {
    
    int head = 85;
    int requests_initial[] = {10, 229, 39, 400, 18, 145, 120, 480, 20, 250};
    int num_requests = sizeof(requests_initial) / sizeof(requests_initial[0]);
    
    
    int requests[num_requests];
    memcpy(requests, requests_initial, sizeof(requests_initial));
    
    qsort(requests, num_requests, sizeof(int), compare_int);

    printf("--- Disk Scheduling Simulation ---\n");
    printf("Disk Size: %d to %d\n", CYLINDER_MIN, CYLINDER_MAX);
    printf("Initial Head Position: %d\n", head);
    printf("Sorted Requests: ");
    print_array(requests, num_requests);
    printf("Initial Movement: Towards %d (UP)\n", CYLINDER_MAX);
    printf("--------------------------------------\n\n");

    
    int split_point_index = 0;
    int i;
    for (i = 0; i < num_requests; i++) {
        if (requests[i] >= head) {
            split_point_index = i;
            break;
        }
    }

    
    int choice;
    do {
        printf("\n--- Choose an Algorithm --- \n");
        printf("1. SCAN\n");
        printf("2. LOOK\n");
        printf("3. C-SCAN\n");
        printf("4. C-LOOK\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                algorithm_scan(head, requests, num_requests, split_point_index);
                break;
            case 2:
                algorithm_look(head, requests, num_requests, split_point_index);
                break;
            case 3:
                algorithm_cscan(head, requests, num_requests, split_point_index);
                break;
            case 4:
                algorithm_clook(head, requests, num_requests, split_point_index);
                break;
            case 0:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);

    return 0;
}




