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
#define MAX_COMMAND_SIZE 8

void controlHandler(int direction);

/* This will make sure that the robot is initially motionless */
int directionController = STOP_CTRL;

int main() {
    char buffer[MAX_COMMAND_SIZE];
    struct sockaddr_in serv_addr, client_addr;
    socklen_t client_addr_size;
    int socketfd, incomingfd, num_of_chars_read;
    pthread_t motor_controller_thread,
              instruction_manager_thread;

    
    /* Setup the socket descriptor */
    socketfd = socket(AF_INET, SOCK_STREAM, 0);

    client_addr_size = sizeof(client_addr);

    /* Initialize our address structures */
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
        printf("The socket has been bound on port: %i\n", CONNECTION_PORT);
    }

    listen(socketfd, 1); /* TODO: use macro for backlog param */

    while( 1 ) {
        incomingfd = accept(socketfd, (struct sockaddr*)&client_addr, 
                &client_addr_size);
        if( incomingfd < 0 ) {
            printf("Failed to accept incoming socket connection.\n");
           continue;
        }

        num_of_chars_read = read( incomingfd, buffer, MAX_COMMAND_SIZE);

        printf("Message of Size %d Recieved: %s\n", num_of_chars_read, buffer);
    }

    /* This return should never be reached */
    return 0;
}
