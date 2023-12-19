#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <string.h>
#include "utils.h"

Server* server;


Command parseMenu(char* input){
    if (input[0] == '\0'){
        return (Command){
                .command = 3,
                .groupid = -1,
                .message = NULL
            };
    }
    char* token = strtok(input, " ");
    char* tokens[3];
    int count = 0;
    while (token != NULL){
        tokens[count] = strdup(token);
        token = strtok(NULL, " ");
        count++;
    }
    for (int i = count; i < 3; i++){
        tokens[count] = NULL;
    }

    if (strcmp(tokens[0], "join") == 0){
        int groupid = atoi(tokens[1]);
        if (!groupid){
            printf("invalid arguement %d for groupid\n", groupid);
            return (Command){
                .command = 3,
                .groupid = -1,
                .message = NULL
            };
        }
        return (Command){0, groupid, NULL};
    }
    else if (strcmp(tokens[0], "send") == 0){
        int groupid = atoi(tokens[1]);
        char* message = tokens[2];
        if (!groupid){
            printf("invalid groupid %d\n", groupid);
            return (Command){
                .command = 3,
                .groupid = -1,
                .message = NULL
            };
        }
        if (message == NULL){
            printf("invalid message: %s\n", message);
            return (Command){
                .command = 3,
                .groupid = -1,
                .message = NULL
            };
        }
        return (Command){1, groupid, message};
    }
    else if (strcmp(tokens[0], "leave") == 0){
        printf("dsadasds");
        fflush(stdout);
        if (tokens[1] == NULL){
            printf("invalid argument");
            return (Command){
                .command = 3,
                .groupid = -1,
                .message = NULL
            };
        }
        int groupid = atoi(tokens[1]);
        if (!groupid){
            printf("invalid arguement %d\n", groupid);
            return (Command){
                .command = 3,
                .groupid = -1,
                .message = NULL
            };
        }
        return (Command){2, groupid, NULL};
    }
    // else if (strcmp(tokens[0], "quit")){
    //     exit(0);
    // }


    return (Command){
                .command = 3,
                .groupid = -1,
                .message = NULL
            };
}


bool contains(int* arr, int item, int* size){
    for (int i = 0; i < *size; i++){
        int comp = arr[i];
        if(comp == item){
            return true;
        }
    }
    return false;
}

bool addToArray(int** arr, int item, int* size) {
    *size += 1;
    *arr = realloc(*arr, (*size) * sizeof(int));
    (*arr)[*size - 1] = item;
    return true;
}


bool deleteFromArray(int** arr, int item, int* size) {
    int idx = -1;
    int length = *size;
    for (int i = 0; i < length; i++) {
        if ((*arr)[i] == item) {
            idx = i;
            break;
        }
    }
    if (idx == -1) {
        return false;
    }
    for (int i = idx; i < length - 1; i++) {
        (*arr)[i] = (*arr)[i + 1];
    }
    *arr = realloc(*arr, (length - 1) * sizeof(int));
    *size -= 1;
    return true;
}

char* joinToGroup(int groupid, int socket){
    
    if (groupid >= 1000 || groupid < 0){
        return false;
    }
    Group* group = ((server->groups)[groupid]);
    char* out = calloc(60, sizeof(char));
    if(contains(group->members, socket, &(group->size))){
        sprintf(out, "the user : %d has already been added to group : %d\n", socket, groupid);
        return out;
    }
    if(addToArray(&(((server->groups)[groupid])->members), socket, &((server->groups)[groupid])->size)){
        sprintf(out, "socket id : %d successfully added to the group : %d\n", socket, groupid);
    }
    else{
        sprintf(out, "something went wrong while joining the group : %d\n", groupid);
    }
    return out;
}
char* leaveFromGroup(int groupid, int socket){
    if (groupid >= 1000 || groupid < 0){
        return false;
    }
    Group* group = ((server->groups)[groupid]);
    char* out = calloc(60, sizeof(char));
    if(deleteFromArray(&(group->members), socket, &group->size)){
        printf("2\n");
        fflush(stdout);
        sprintf(out, "deleted id %d from group\n", socket);
        printf("1\n");
        fflush(stdout);
    }
    else{
        sprintf(out, "something went wrong while processing\n");
    }
    return out;
}

char* sendMessage(int groupid, char* message, int socket){
    if (groupid >= 1000 || groupid < 0){
        return "Invalid group id\n";
    }
    Group* group = ((server->groups)[groupid]);
    if (! contains(group->members, socket, &group->size)){
        return "You must join the group first in order to send the message\n";
    }
    int* size = &group->size;
    for (int i = 0; i < *size; i++){
        send(group->members[i], message, strlen(message), 0);
    }
    return "Message sent\n";
}

void detatchSocket(int socket){
    deleteFromArray(&(server->clients), socket, &(server->clients_size));
    for (int i = 0; i < 1000; i++){
        leaveFromGroup(i, socket);
    }
}