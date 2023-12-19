mkdir -p client
gcc -c client.c -o client/client.o
gcc -c utils.c -o client/utils.o
gcc -o client/main client/utils.o client/client.o
./client/main