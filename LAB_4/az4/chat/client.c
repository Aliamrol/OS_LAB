#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "utils.h"
#define PORT 8080



void showMenu(){
    printf("----    HOW TO USE    ----\n\n");
    printf("join  [GROUPID]\n");
    printf("send  [GROUPID] [MESSAGE]\n");
    printf("leave [GROUPID]\n");
}


char* request(int sock, char* buffer, int len){
    char* ans = calloc(1024, sizeof(char));
    send(sock, buffer, strlen(buffer), 0);
    read(sock, ans, 1024);
    return ans;
}



int main(int argc, char const *argv[]) {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
    
    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0){
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
        printf("\nConnection Failed \n");
        return -1;
    }


    while(true){
        showMenu();
        char input[1024];
        fgets(input, 1024, stdin);
        if (strcmp(input, "quit") == 0){
            exit(0);
        }
        char* ans = request(sock, input, 1024);
        printf("\033[32m%s\n\033[0m", ans);
    }
    return 0;
}



