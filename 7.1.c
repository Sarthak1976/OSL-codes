// sender.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h> 

// Define a key for our message queue
#define MSG_KEY 1234

// Define the message structure
// It MUST start with a long integer (msg_type)
struct msg_buffer {
    long msg_type;
    char msg_text[100];
};

int main() {
    int msqid;
    struct msg_buffer message;

    // 1. Get the message queue ID
    // IPC_CREAT: Create the queue if it doesn't exist.
    // 0666: Read/write permissions.
    msqid = msgget(MSG_KEY, 0666 | IPC_CREAT);
    if (msqid == -1) {
        perror("msgget failed");
        exit(1);
    }

    printf("Sender is ready. (msqid: %d)\n", msqid);
    printf("Enter 'exit' to stop.\n");

    // 2. Loop to get messages from the user
    while (1) {
        // Get message type from user
        printf("\nEnter message type (a number > 0): ");
        scanf("%ld", &message.msg_type);

        // Clear the input buffer (to handle the newline after scanf)
        while (getchar() != '\n'); 

        // Get message text from user
        printf("Enter message text: ");
        fgets(message.msg_text, 100, stdin);
        
        // Remove the trailing newline character from fgets
        message.msg_text[strcspn(message.msg_text, "\n")] = 0;

        // 3. Send the message
        if (msgsnd(msqid, &message, sizeof(message.msg_text), 0) == -1) {
            perror("msgsnd failed");
            exit(1);
        }

        printf("Sent: '%s' (Type: %ld)\n", message.msg_text, message.msg_type);

        // 4. Check for exit condition
        if (strcmp(message.msg_text, "exit") == 0) {
            break;
        }
    }

    printf("Sender is exiting.\n");
    return 0;
}





// receiver.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <ctype.h> // For toupper()

// Use the SAME key as the sender
#define MSG_KEY 1234

// Use the SAME message structure
struct msg_buffer {
    long msg_type;
    char msg_text[100];
};

// Helper function to convert a string to uppercase
void to_upper(char *str) {
    while (*str) {
        *str = toupper((unsigned char)*str);
        str++;
    }
}

int main() {
    int msqid;
    struct msg_buffer message;

    // 1. Get the message queue ID (same key)
    msqid = msgget(MSG_KEY, 0666);
    if (msqid == -1) {
        perror("msgget failed");
        exit(1);
    }

    printf("Receiver is ready. Waiting for messages... (msqid: %d)\n", msqid);

    // 2. Loop to receive messages
    while (1) {
        // 3. Receive a message
        // The 4th argument (0) means "receive the first available message" (FIFO)
        if (msgrcv(msqid, &message, sizeof(message.msg_text), 0, 0) == -1) {
            perror("msgrcv failed");
            exit(1);
        }

        // 4. Check for exit condition
        if (strcmp(message.msg_text, "exit") == 0) {
            printf("Received 'exit' message. Shutting down.\n");
            break;
        }

        // 5. Process the message (convert to uppercase)
        printf("Received (Type: %ld): %s\n", message.msg_type, message.msg_text);
        to_upper(message.msg_text);
        printf("Processed (UPPER): %s\n\n", message.msg_text);
    }

    // 6. Clean up: Remove the message queue
    if (msgctl(msqid, IPC_RMID, NULL) == -1) {
        perror("msgctl(IPC_RMID) failed");
        exit(1);
    }

    printf("Message queue cleaned up.\n");
    return 0;
}

Compile: gcc sender.c -o sender gcc receiver.c -o receiver

Run in two terminals: ./receiver (in Terminal 1) ./sender (in Terminal 2)
