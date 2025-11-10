#include <stdio.h>
#include <stdbool.h> // Lets us use 'true' and 'false'

// Define max sizes. It's easier than dynamic allocation.
#define MAX_P 10
#define MAX_R 10

// --- Global Variables ---
// We make these global so all functions can see them.
int P, R;                         // Number of Processes and Resources
int allocation[MAX_P][MAX_R];
int max[MAX_P][MAX_R];
int available[MAX_R];
int need[MAX_P][MAX_R];
int safeSequence[MAX_P];

// --- Prototype Functions ---
// We must declare the functions here so main() knows they exist.
void calculateNeed();
bool isSafeState();
void resourceRequest();

//==================================================
//  MAIN FUNCTION (The "Manager")
//==================================================
int main() {
    
    // --- 1. Read Data From File ---
    FILE *fp; // This is a "file pointer"
    
    // "fopen" means "file open". "r" means "read-only"
    fp = fopen("state.txt", "r"); 
    
    if (fp == NULL) {
        printf("Error: Could not open file 'state.txt'.\n");
        printf("Please create the file in the same directory.\n");
        return 1; // Exit with an error
    }

    // "fscanf" is like "scanf" but for files.
    // Read P and R
    fscanf(fp, "%d %d", &P, &R);

    // Read Max matrix
    for (int i = 0; i < P; i++) {
        for (int j = 0; j < R; j++) {
            fscanf(fp, "%d", &max[i][j]);
        }
    }

    // Read Allocation matrix
    for (int i = 0; i < P; i++) {
        for (int j = 0; j < R; j++) {
            fscanf(fp, "%d", &allocation[i][j]);
        }
    }

    // Read Available vector
    for (int i = 0; i < R; i++) {
        fscanf(fp, "%d", &available[i]);
    }

    // "fclose" means "file close". Always do this!
    fclose(fp); 
    printf("Data loaded from 'state.txt' (P=%d, R=%d)\n", P, R);
    
    // --- 2. Calculate Need ---
    calculateNeed();

    // --- 3. Requirement 5.2.1: Check initial state ---
    printf("\n--- 1. Checking Initial State ---\n");
    isSafeState();

    // --- 4. Requirement 5.2.2: Demonstrate resource request ---
    printf("\n--- 2. Demonstrate Resource Request (for GRANT) ---\n");
    resourceRequest();
    
    // --- 5. Requirement 5.2.2: Demonstrate another request ---
    printf("\n--- 3. Demonstrate Resource Request (for NON-GRANT) ---\n");
    resourceRequest();

    return 0; // Program finished successfully
}

//==================================================
//  HELPER FUNCTIONS
//==================================================

/**
 * Calculates the 'Need' matrix.
 * Need = Max - Allocation
 */
void calculateNeed() {
    for (int i = 0; i < P; i++) {
        for (int j = 0; j < R; j++) {
            // This is the data validation step from your assignment image
            // which shows Allocation > Max. This is an error in the
            // assignment's data, not the algorithm.
            if (max[i][j] < allocation[i][j]) {
                 printf("Warning: P%d Allocation (%d) > Max (%d) for R%d.\n",
                        i, allocation[i][j], max[i][j], j);
                 need[i][j] = 0; // Set need to 0 if data is bad
            } else {
                 need[i][j] = max[i][j] - allocation[i][j];
            }
        }
    }
}

/**
 * The Safety Algorithm (5.2.1)
 * Checks if the *current* system state is safe.
 * Returns true (1) if safe, false (0) if unsafe.
 */
bool isSafeState() {
    int work[MAX_R];
    bool finish[MAX_P] = {false};
    int count = 0;

    for (int i = 0; i < R; i++) {
        work[i] = available[i];
    }

    for (int pass = 0; pass < P; pass++) {
        for (int i = 0; i < P; i++) {
            if (finish[i] == false) {
                bool canAllocate = true;
                for (int j = 0; j < R; j++) {
                    if (need[i][j] > work[j]) {
                        canAllocate = false;
                        break;
                    }
                }
                if (canAllocate) {
                    for (int j = 0; j < R; j++) {
                        work[j] += allocation[i][j];
                    }
                    finish[i] = true;
                    safeSequence[count++] = i;
                }
            }
        }
    }

    if (count == P) {
        printf("\n✅ System is in a SAFE STATE.\n");
        printf("   Safe Sequence: ");
        for (int i = 0; i < P; i++) {
            printf("P%d", safeSequence[i]);
            if (i < P - 1) printf(" -> ");
        }
        printf("\n");
        return true;
    } else {
        printf("\n❌ System is in an UNSAFE STATE.\n");
        return false;
    }
}

/**
 * The Resource-Request Algorithm (5.2.2)
 * Checks if a "what if" request can be safely granted.
 */
void resourceRequest() {
    int p_id;
    int request[MAX_R];

    printf("Enter process ID (e.g., 0 for P0): ");
    scanf("%d", &p_id);

    printf("Enter request vector (e.g., '0 1 0 2'): ");
    for (int i = 0; i < R; i++) {
        scanf("%d", &request[i]);
    }

    // --- Step 1: Check if Request <= Need ---
    for (int i = 0; i < R; i++) {
        if (request[i] > need[p_id][i]) {
            printf("\n❌ Request DENIED. Process P%d is asking for more than its max need.\n", p_id);
            return; // Exit the function
        }
    }

    // --- Step 2: Check if Request <= Available ---
    for (int i = 0; i < R; i++) {
        if (request[i] > available[i]) {
            printf("\n❌ Request DENIED. Resources not available. Process P%d must wait.\n", p_id);
            return; // Exit the function
        }
    }

    // --- Step 3: "Pretend" to grant the request ---
    // Save the old state, just in case
    int old_available[MAX_R];
    int old_allocation[MAX_R];
    int old_need[MAX_R];
    for (int i = 0; i < R; i++) {
        old_available[i] = available[i];
        old_allocation[i] = allocation[p_id][i];
        old_need[i] = need[p_id][i];
        
        // Grant the request (temporarily)
        available[i] -= request[i];
        allocation[p_id][i] += request[i];
        need[p_id][i] -= request[i];
    }

    printf("\n...Running safety check on this hypothetical state...\n");

    // --- Step 4: Run the Safety Algorithm ---
    if (isSafeState()) {
        // --- Step 5 (Safe): Grant the request ---
        printf("\n✅ Request GRANTED. State is safe.\n");
        // We keep the changes.
    } else {
        // --- Step 5 (Unsafe): Deny the request ---
        printf("\n❌ Request DENIED. Granting would lead to an unsafe state.\n");
        printf("...Rolling back to the previous safe state...\n");
        
        // We must roll back!
        for (int i = 0; i < R; i++) {
            available[i] = old_available[i];
            allocation[p_id][i] = old_allocation[i];
            need[p_id][i] = old_need[i];
        }
    }
}
