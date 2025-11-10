// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h> // For sleep()

// Define a key for the shared memory
#define SHM_KEY 6168

// Define the shared structure (for data and synchronization)
struct shared_buffer {
    int server_ready; // 0 = not ready, 1 = ready
    int client_ready; // 0 = not ready, 1 = ready
    char message[200];
};

int main() {
    int shmid;
    void *shared_memory;
    struct shared_buffer *shm_ptr;

    // 1. Create the shared memory segment
    // We use ftok to create a key from a file, which is more reliable
    key_t key = ftok("keyfile", 'S'); // Create a file named 'keyfile' first!
    if (key == -1) {
        perror("ftok");
        exit(1);
    }
    
    shmid = shmget(key, sizeof(struct shared_buffer), 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget failed");
        exit(1);
    }
    printf("Server: Shared memory segment created.\n");

    // 2. Attach the shared memory
    shared_memory = shmat(shmid, NULL, 0);
    if (shared_memory == (void *)-1) {
        perror("shmat failed");
        exit(1);
    }
    printf("Server: Shared memory attached.\n");

    // 3. Cast the pointer
    shm_ptr = (struct shared_buffer *)shared_memory;

    // 4. Initialize flags and write message
    shm_ptr->server_ready = 0;
    shm_ptr->client_ready = 0;
    strcpy(shm_ptr->message, "Hello from the Server!");

    // 5. Set ready flag (Synchronization)
    shm_ptr->server_ready = 1;
    printf("Server: Message written. Waiting for client...\n");

    // 6. Wait for client to be ready (Synchronization)
    while (shm_ptr->client_ready == 0) {
        sleep(1);
    }

    // 7. Detach the shared memory
    shmdt(shared_memory);

    // 8. Remove the shared memory segment (Cleanup)
    shmctl(shmid, IPC_RMID, NULL);

    printf("Server: Client finished. Shared memory cleaned up.\n");
    return 0;
}


// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h> // For sleep()

// Use the SAME key
#define SHM_KEY 6168

// Use the SAME structure
struct shared_buffer {
    int server_ready;
    int client_ready;
    char message[200];
};

int main() {
    int shmid;
    void *shared_memory;
    struct shared_buffer *shm_ptr;

    // 1. Get the shared memory segment
    // Use the same ftok logic to find the key
    key_t key = ftok("keyfile", 'S'); // Use same file and ID
    if (key == -1) {
        perror("ftok");
        exit(1);
    }

    shmid = shmget(key, sizeof(struct shared_buffer), 0666);
    if (shmid == -1) {
        perror("shmget failed");
        exit(1);
    }
    printf("Client: Found shared memory segment.\n");

    // 2. Attach the shared memory
    shared_memory = shmat(shmid, NULL, 0);
    if (shared_memory == (void *)-1) {
        perror("shmat failed");
        exit(1);
    }
    printf("Client: Shared memory attached.\n");

    // 3. Cast the pointer
    shm_ptr = (struct shared_buffer *)shared_memory;

    // 4. Wait for server to be ready (Synchronization)
    printf("Client: Waiting for server...\n");
    while (shm_ptr->server_ready == 0) {
        sleep(1);
    }

    // 5. Read the message
    printf("Client: Message from server is: '%s'\n", shm_ptr->message);

    // 6. Signal back to server (Synchronization)
    shm_ptr->client_ready = 1;
    printf("Client: Signal sent back to server.\n");

    // 7. Detach the shared memory
    shmdt(shared_memory);

    printf("Client: Exiting.\n");
    return 0;
}


Create the key file: touch keyfile

Compile: gcc server.c -o server gcc client.c -o client

Run in two terminals: ./server (in Terminal 1) ./client (in Terminal 2)
