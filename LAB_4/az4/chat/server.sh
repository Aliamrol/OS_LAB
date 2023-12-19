mkdir -p server
gcc -c server.c -o server/server.o
gcc -c utils.c -o server/utils.o
gcc -o server/main server/server.o server/utils.o
./server/main