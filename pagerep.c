#include <stdio.h>
#include <stdlib.h> 
#include <time.h>   
#include <stdbool.h>



bool isPageInFrames(int page, int frames[], int num_frames) {
    for (int i = 0; i < num_frames; i++) {
        if (frames[i] == page) {
            return true;
        }
    }
    return false;
}


int findOptimalVictim(int frames[], int num_frames, int refString[], int length, int startIndex) {
    int victim_index = -1;
    int furthest_use = -1; 

   
    for (int i = 0; i < num_frames; i++) {
        int currentPageInFrame = frames[i];
        int next_use = -1;

        
        for (int j = startIndex; j < length; j++) {
            if (refString[j] == currentPageInFrame) {
                next_use = j; 
                break;
            }
        }

        
        if (next_use == -1) {
            return i; 
        }

        
        if (next_use > furthest_use) {
            furthest_use = next_use;
            victim_index = i;
        }
    }
    return victim_index; 
}


int findLruVictim(int recency[], int num_frames) {
    int lru_index = 0;
    int min_recency = recency[0];

    for (int i = 1; i < num_frames; i++) {
        if (recency[i] < min_recency) {
            min_recency = recency[i];
            lru_index = i;
        }
    }
    return lru_index; 
}


int runFIFO(int refString[], int length, int num_frames) {
    int frames[num_frames];
    int page_faults = 0;
    int replace_index = 0; 

    
    for (int i = 0; i < num_frames; i++) {
        frames[i] = -1;
    }

    
    for (int i = 0; i < length; i++) {
        int currentPage = refString[i];

        if (isPageInFrames(currentPage, frames, num_frames)) {
            
            continue;
        }

        
        page_faults++;
        
        
        frames[replace_index] = currentPage;
        
        
        replace_index = (replace_index + 1) % num_frames;
    }
    return page_faults;
}


int runLRU(int refString[], int length, int num_frames) {
    int frames[num_frames];
    int recency[num_frames]; // Array to track "age"
    int page_faults = 0;
    int time_counter = 0; // Our "clock"

    for (int i = 0; i < num_frames; i++) {
        frames[i] = -1;  // Empty
        recency[i] = 0; // Initial age
    }

    for (int i = 0; i < length; i++) {
        time_counter++; // Clock ticks with every page reference
        int currentPage = refString[i];

        int found_index = -1;
        for (int j = 0; j < num_frames; j++) {
            if (frames[j] == currentPage) {
                found_index = j;
                break;
            }
        }

        if (found_index != -1) {
            
            recency[found_index] = time_counter;
            continue;
        }

        
        page_faults++;

        int empty_index = -1;
        for (int j = 0; j < num_frames; j++) {
            if (frames[j] == -1) {
                empty_index = j;
                break;
            }
        }

        if (empty_index != -1) {
            frames[empty_index] = currentPage;
            recency[empty_index] = time_counter;
        } else {
            
            int lru_victim_index = findLruVictim(recency, num_frames);
            frames[lru_victim_index] = currentPage;
            recency[lru_victim_index] = time_counter;
        }
    }
    return page_faults;
}

int runOptimal(int refString[], int length, int num_frames) {
    int frames[num_frames];
    int page_faults = 0;

    for (int i = 0; i < num_frames; i++) {
        frames[i] = -1;
    }

    for (int i = 0; i < length; i++) {
        int currentPage = refString[i];

        if (isPageInFrames(currentPage, frames, num_frames)) {
            // "Page Hit". Do nothing.
            continue;
        }

        // "Page Fault".
        page_faults++;

        // Check for an empty frame first
        int empty_index = -1;
        for (int j = 0; j < num_frames; j++) {
            if (frames[j] == -1) {
                empty_index = j;
                break;
            }
        }

        if (empty_index != -1) {
            // Found an empty frame. Use it.
            frames[empty_index] = currentPage;
        } else {
            // No empty frames. Must replace the *Optimal* page.
            // (The one used furthest in the future)
            int opt_victim_index = findOptimalVictim(frames, num_frames, refString, length, i + 1);
            frames[opt_victim_index] = currentPage;
        }
    }
    return page_faults;
}


// --- Main Function to drive the simulation ---
int main() {
    // Seed the random number generator
    srand(time(NULL));

    // --- 1. Generate the Random Page-Reference String ---
    int string_length = 20; // Let's use a string of 20 pages
    int refString[string_length];

    printf("--- Page Replacement Simulation ---\n\n");
    printf("Generated Page-Reference String:\n");
    for (int i = 0; i < string_length; i++) {
        refString[i] = rand() % 10; // Random page number from 0-9
        printf("%d ", refString[i]);
    }
    printf("\n\n");

    // --- 2. Interactive Menu using a switch ---
    int choice;
    do {
        printf("--- Main Menu ---\n");
        printf("1. FIFO\n");
        printf("2. LRU\n");
        printf("3. Optimal\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        int num_frames;
        int faults;

        switch (choice) {
            case 1: // Run FIFO
                printf("\n  Run FIFO:\n");
                printf("  Enter number of frames (1-7): ");
                scanf("%d", &num_frames);
                if (num_frames < 1 || num_frames > 7) {
                    printf("  Invalid number of frames.\n\n");
                    break;
                }
                faults = runFIFO(refString, string_length, num_frames);
                printf("  => FIFO with %d frames had %d page faults.\n\n", num_frames, faults);
                break;

            case 2: // Run LRU
                printf("\n  Run LRU:\n");
                printf("  Enter number of frames (1-7): ");
                scanf("%d", &num_frames);
                if (num_frames < 1 || num_frames > 7) {
                    printf("  Invalid number of frames.\n\n");
                    break;
                }
                faults = runLRU(refString, string_length, num_frames);
                printf("  => LRU with %d frames had %d page faults.\n\n", num_frames, faults);
                break;

            case 3: // Run Optimal
                printf("\n  Run Optimal:\n");
                printf("  Enter number of frames (1-7): ");
                scanf("%d", &num_frames);
                if (num_frames < 1 || num_frames > 7) {
                    printf("  Invalid number of frames.\n\n");
                    break;
                }
                faults = runOptimal(refString, string_length, num_frames);
                printf("  => Optimal with %d frames had %d page faults.\n\n", num_frames, faults);
                break;

            case 4: // Exit
                printf("\nExiting program.\n");
                break;

            default:
                printf("\nInvalid choice. Please try again.\n\n");
                break;
        }

    } while (choice != 4);

    return 0;
}




//main2

// --- Main Function to drive the simulation ---
// int main() {
//     // --- 1. Define the specific Page-Reference String ---
//     // User said size 12, but the list has 13 pages.
//     int string_length = 13;
//     int refString[] = {1, 2, 3, 4, 1, 2, 5, 1, 1, 2, 3, 4, 5};

//     printf("--- Page Replacement Simulation ---\n\n");
//     printf("Reference String: 1, 2, 3, 4, 1, 2, 5, 1, 1, 2, 3, 4, 5\n\n");

//     // --- 2. Run simulation for 3 frames ---
//     int num_frames_3 = 3;
//     printf("--- Results for Frame Size %d ---\n", num_frames_3);

//     int fifo_faults_3 = runFIFO(refString, string_length, num_frames_3);
//     int lru_faults_3 = runLRU(refString, string_length, num_frames_3);
//     int opt_faults_3 = runOptimal(refString, string_length, num_frames_3);

//     printf("FIFO Page Faults: %d\n", fifo_faults_3);
//     printf("LRU Page Faults:  %d\n", lru_faults_3);
//     printf("Optimal Page Faults: %d\n", opt_faults_3);
    
//     // --- 3. Run simulation for 4 frames ---
//     int num_frames_4 = 4;
//     printf("\n--- Results for Frame Size %d ---\n", num_frames_4);

//     int fifo_faults_4 = runFIFO(refString, string_length, num_frames_4);
//     int lru_faults_4 = runLRU(refString, string_length, num_frames_4);
//     int opt_faults_4 = runOptimal(refString, string_length, num_frames_4);

//     printf("FIFO Page Faults: %d\n", fifo_faults_4);
//     printf("LRU Page Faults:  %d\n", lru_faults_4);
//     printf("Optimal Page Faults: %d\n", opt_faults_4);

//     return 0;
// }
