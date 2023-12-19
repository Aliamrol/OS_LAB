// Forward declaration for Group
typedef struct {
    int* members;
    int size;
} Group;

// Definition for Command
typedef struct {
    int command;
    int groupid;
    char* message;
} Command;

// Definition for Response
typedef struct {
    int code;
    char* message;
    int port;
} Response;

// Declaration for parseMenu
Command parseMenu(char*);

// Definition for Server
typedef struct {
    int* clients;
    int clients_size;
    Group** groups;
    int groups_size;
} Server;

// Function Declarations
extern Server* server;

Command parseMenu(char*);
bool addToArray(int**, int, int*);
bool deleteFromArray(int**, int, int*);
char* joinToGroup(int, int);
char* leaveFromGroup(int, int);
char* sendMessage(int, char*, int);
void detatchSocket(int);