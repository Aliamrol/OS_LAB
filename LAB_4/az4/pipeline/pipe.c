#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

char originalMessage[28] = "UsinG CurreNt WorLd";

int main() {
    int pipefdParentToChild[2];
    int pipefdChildToParent[2];

    pid_t childPid;

    if (pipe(pipefdParentToChild) == -1) {
        perror("error creating pipe1");
        exit(EXIT_FAILURE);
    }

    if (pipe(pipefdChildToParent) == -1) {
        perror("error creating pipe2");
        exit(EXIT_FAILURE);
    }

    childPid = fork();

    if (childPid == -1) {
        perror("failed to fork the process");
        exit(EXIT_FAILURE);
    }

    if (childPid != 0) {
        close(pipefdParentToChild[0]);
        close(pipefdChildToParent[1]);
        printf("%s\n", originalMessage);
        write(pipefdParentToChild[1], originalMessage, sizeof(originalMessage));
        close(pipefdParentToChild[1]);

        wait(NULL);

        read(pipefdChildToParent[0], originalMessage, sizeof(originalMessage));
        close(pipefdChildToParent[0]);
        printf("Changed message is: %s\n", originalMessage);
    } else {
        close(pipefdParentToChild[1]);
        close(pipefdChildToParent[0]);
        char changedMessage[strlen(originalMessage)];
        read(pipefdParentToChild[0], changedMessage, sizeof(changedMessage));
        close(pipefdParentToChild[0]);

        for (int i = 0; i < strlen(changedMessage); i++) {
            if (islower(changedMessage[i])) {
                changedMessage[i] = toupper(changedMessage[i]);
            } else {
                changedMessage[i] = tolower(changedMessage[i]);
            }
        }

        write(pipefdChildToParent[1], changedMessage, sizeof(changedMessage));
        close(pipefdChildToParent[1]);
    }

    return 0;
}