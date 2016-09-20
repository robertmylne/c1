#include <elf.h>
#include <memory.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER 80

int main(int argc, char *argv[])
{
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sock, s, j;
    size_t len;
    ssize_t nread;
    char buffer[BUFFER];

    // Obtain address matching the host / port
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    hints.ai_protocol = 0;

    // "127.0.0.1", "80", service requested, successful results stored in result
    // getaddrinfo() returns a list of address structures
    s = getaddrinfo(argv[1], argv[2], &hints, &result);

    // Try each address until we connect, if it fails close the socket and try again
    for(rp = result; rp != NULL; rp = rp->ai_next) {
        sock = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if(sock == -1) {
            continue;
        }
        if(connect(sock, rp->ai_addr, rp->ai_addrlen) != -1) {
            break;
        }
        close(sock);
    }

    freeaddrinfo(result);

    char input[BUFFER];
    printf("> ");
    while(fgets(input, BUFFER, stdin) != NULL) {

        // Check if user wants to exit
        if(strcmp(strtok(input, "\n"), "quit") == 0) {
            printf("Goodbye :)");
            break;
        }

        time_t start_timer, finish_timer;

        // Start timer
        time(&start_timer);

        // Send request to server
        write(sock, input, BUFFER);

        // Read response from server
        nread = read(sock, buffer, BUFFER);
        if(nread == -1) {
            perror("Read");
            exit(EXIT_FAILURE);
        }

        // Finish timer
        time(&finish_timer);

        // Send server processing time to client
        printf("# Time: %f \n", difftime(finish_timer, start_timer));

        // Print server response to shell
        printf("%s \n", buffer);

        // Wait for next user command
        printf("> ");
    }

    exit(EXIT_SUCCESS);
}