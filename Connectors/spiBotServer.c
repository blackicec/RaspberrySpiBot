#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/unistd.h>
#include <pthread.h>
#include "./motorController.h"

#define CONNECTION_PORT 1550
#define MAX_COMMAND_SIZE 10

void* instruction_manager_handler();
void* motor_control_handler();

const char* forward_command = "forward";
const char* reverse_command = "reverse";
const char* turn_left_command = "turn_left";
const char* turn_right_command = "turn_right";

/* 
* If our socket status is anything other than 0, then our two threads will 
* do any cleanup, stop the vehicle force the program to exit
*/
int socket_status = 0;

typedef struct instruction {
    int instruction_code;
    time_t last_modified;
} instruction;

instruction motor_instruction;

int main() {
    int thread_creation_status;
    pthread_t instruction_manager_thread,
        motor_controller_thread;

    /* Setup our GPIO pin configuration */
    char* configuration_file_location;

    configuration_file_location = "*"; /* This will eventually ba a real file */

    /* Initialize our motor instructions structure */
    motor_instruction.instruction_code = STOP_CTRL;
    motor_instruction.last_modified = time(NULL);

    motor_pin_setup(configuration_file_location);

    /* Fire up our thread so they can get to work */
    thread_creation_status = pthread_create(&instruction_manager_thread,
        NULL, &instruction_manager_handler, NULL);

    if(thread_creation_status != 0) {
        printf("An error occurred while creating the Instruction Manager thread.\n");
        return -1;
    }

    thread_creation_status = pthread_create(&motor_controller_thread,
        NULL, &motor_control_handler, NULL);

    if(thread_creation_status != 0) {
        printf("An error occurred while creating the Motor Controller thread.\n");

        /* 
        * Since the Instruction Manager thread was created successfully
        * at this point, we need to clean it up before ending this execution.
        */
        pthread_exit(&instruction_manager_thread);
        return -1;
    }

    pthread_join(instruction_manager_thread, NULL);
    pthread_join(motor_controller_thread, NULL);

    /* This return should never be reached */
    printf("Execution Complete.\n");
    return 0;
}

void* instruction_manager_handler( void* arg ) {
    char buffer[MAX_COMMAND_SIZE];
    struct sockaddr_in serv_addr, client_addr;
    socklen_t client_addr_size;
    int socketfd, incomingfd, num_of_chars_read;
        
    printf("Instruction Manager has started successfully.\n");
    
    /* Setup the socket descriptor */
    socketfd = socket(AF_INET, SOCK_STREAM, 0);

    client_addr_size = sizeof(client_addr);

    /* Initialize our server and client address structures */
    memset((char*)&serv_addr, 0, sizeof(serv_addr));
    memset((char*)&client_addr, 0, client_addr_size);
    memset(buffer, 0, MAX_COMMAND_SIZE);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(CONNECTION_PORT);
   
    /* Attempt to bind the socket to the socket descriptor */
    if ( bind(socketfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Failed to bind to socket: %i\n", CONNECTION_PORT);
    }
    else {
        /* TODO: Send data like this to a log file and not to stdout */
        printf("The socket has been bound on port: %i\n", CONNECTION_PORT);
    }

    ListenForConnection:
        socket_status = listen(socketfd, 1); 

        if(socket_status != 0) {
            printf("Failed to listen on port: %i\n", CONNECTION_PORT);
            return arg; /* For lack of anything better to return right now */
        }

        while( 1 ) {
            incomingfd = accept(socketfd, (struct sockaddr*)&client_addr, 
                    &client_addr_size);

            /* 
            * If something is wrong with our message, then we will go back 
            * and check our socket connection status.
            */
            if( incomingfd < 0 ) {
               printf("Failed to accept incoming socket communication.\n");
               goto ListenForConnection;
            }

            num_of_chars_read = read( incomingfd, buffer, MAX_COMMAND_SIZE);

            printf("Message of Size %d Recieved: %s\n", num_of_chars_read, buffer);

            /*
            * Set the global motor_instruction structure so motor_controller
            * thread can handle it accordingly. If the instruction is the same
            * as the previous, the motor_controller will just ignore it.
            */
            if(strcmp(buffer, forward_command) == 0) {
                motor_instruction.instruction_code = FORWARD_CTRL;
                motor_instruction.last_modified = time(NULL);
            } else if(strcmp(buffer, reverse_command) == 0) {
                motor_instruction.instruction_code = REVERSE_CTRL;
                motor_instruction.last_modified = time(NULL);
            } else if(strcmp(buffer, turn_left_command) == 0) {
                motor_instruction.instruction_code = LEFT_CTRL;
                motor_instruction.last_modified = time(NULL);
            } else if(strcmp(buffer, turn_right_command) == 0) {
                motor_instruction.instruction_code = RIGHT_CTRL;
                motor_instruction.last_modified = time(NULL);
            } else {
                motor_instruction.instruction_code = STOP_CTRL;
                motor_instruction.last_modified = time(NULL);
            }

            printf("Instruction modified at: %li seconds.\n",
                motor_instruction.last_modified);

            /* Clean out the buffer */
            memset(buffer, 0, MAX_COMMAND_SIZE);
        }
}

void* motor_control_handler( void* arg ) {
    /* 
    * Give the controller its first instruction . . . make sure it's stopped.
    * This will make sure that the robot is initially motionless.
    */
    int current_instruction;

    current_instruction = STOP_CTRL;
    instruction_handler(current_instruction);
    
    printf("Thread Motor Controller has started successfully.\n");

    while( 1 ) {

        if(motor_instruction.last_modified + 1 >= time(NULL) 
            && motor_instruction.instruction_code != current_instruction) {
            
            instruction_handler(motor_instruction.instruction_code);
            current_instruction = motor_instruction.instruction_code;

            printf("New instruction has been recieved and handled: %i\n",
                motor_instruction.instruction_code);

            /* Allow this thread to take a little break before checking again */
            /*sleep(1);  This may be too long of a sleep, but its worth a try */
        }
    }
}
