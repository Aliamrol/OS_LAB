#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include "utils.h"
#define PORT 8080


typedef struct {
    int thread_id;
    int socket;
} ThreadInfo;

void* handleRequest(void* args){
    char buffer[1024];
    
    ThreadInfo* info = (ThreadInfo*) args;
    int socket = info->socket;
    
    while(true){
        int c = recv(socket, buffer, 1024, 0);
        if (c == 0){
            pthread_exit(NULL);
        }
        Command command = parseMenu(buffer);
        char* message = "--Error--";
        switch (command.command)
        {
        case 0:
            message = joinToGroup(command.groupid, socket);
            break;
        
        case 1:
            message = sendMessage(command.groupid, command.message, socket);
            break;

        case 2:
            message = leaveFromGroup(command.groupid, socket);
            break;
        case 3:
            message = "something went wrong while processing the request\n";
            break;
        }
        for (int i = 0; i < sizeof(buffer)/sizeof(char); i++){
            buffer[i] = '\0';
        }
        send(socket, message, strlen(message), 0);
    }
    

    
}

int main() {

    server = (Server*) calloc(1, sizeof(Server));

    server->groups = (Group**) calloc(1000, sizeof(Group*));
    for (int i = 0; i < 1000; i++){
        server->groups[i] = (Group*) calloc(0, sizeof(Group*));
        (server->groups)[i]->size = 0;
        (server->groups[i])->members = (int*) calloc(0, sizeof(int));
    }

    int server_fd;
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    const int addrlen = sizeof(address);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("the port has been successfully bounded to the process\n");
    

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("listenning on port : %d\n", PORT);
    

    while(true){
        int client_socket;
        if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            printf("an error has occured\n");
            perror("accept");
            exit(EXIT_FAILURE);
        }
        printf("client %d successfully connected to the server\n", client_socket);
        pthread_t threadid;
        ThreadInfo* info = (ThreadInfo*) calloc(1, sizeof(ThreadInfo));
        info->socket = client_socket;

        if ((pthread_create(&threadid, NULL, handleRequest, (void*)info)) < 0){
            perror("Thread creation failed\n");
            exit(EXIT_FAILURE);
        }

    }

    return 0;
}





