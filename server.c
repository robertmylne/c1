#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/socket.h>
#include <cygwin/in.h>
#include <sys/unistd.h>
#include <string.h>

#define PORT 80
#define BUFFER 80

void service_client(int connection, char input[BUFFER], char output[BUFFER]);

int main()
{
    int sock, connection, client_length;
    struct sockaddr_in server, client;

    // Zombie killer
    signal(SIGCHLD, SIG_IGN);

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0) {
        perror("Creating stream socket");
        exit(1);
    }

    // Create server
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);

    // Bind port
    if(bind(sock, (struct sockaddr*) &server, sizeof(server)) < 0) {
        perror("Binding socket");
        exit(2);
    }

    // Listen
    listen(sock, 5);
    printf("Listening on port %i \n", PORT);

    while(1)
    {
        // Messages
        char input[BUFFER], output[BUFFER];

        // Accept
        client_length = sizeof(client);
        connection = accept(sock, (struct sockaddr*) &client, &client_length);
        if(connection < 0) {
            perror("Accepting connection");
            exit(3);
        }

        // Connection
        if(fork() == 0) {
            close(sock);
            printf("Client connected: (pid %d) | Using descriptor: %d \n", getpid(), sock);

            service_client(connection, input, output);

            printf("Client exited: (pid %d) \n", getpid());
            exit(0);
        }

        // Close connection
        close(connection);
    }
}

void service_client(int connection, char input[BUFFER], char output[BUFFER])
{
    // While requests are coming in
    while(read(connection, input, BUFFER))
    {
        sprintf(output, "%s", input);
        if(fork() == 0) {
            execlp("ls", "ls", NULL);
            // get the execlp output
        }
        write(connection, output, strlen(output));
    }
}