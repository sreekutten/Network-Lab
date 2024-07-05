#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8081
#define MAXLINE 1024

int main() {
    int sockfd;
    char buffer[MAXLINE];
    char message[20];
    struct sockaddr_in servaddr;

    while (1) {
        printf("\nEnter data to send: ");
        fgets(message, sizeof(message), stdin);
        message[strcspn(message, "\n")] = '\0';

        if (strcmp(message, "exit") == 0)
            break;

        if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
            perror("socket creation failed");
            exit(EXIT_FAILURE);
        }

        memset(&servaddr, 0, sizeof(servaddr));

        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(PORT);
        servaddr.sin_addr.s_addr = INADDR_ANY;

        sendto(sockfd, (const char *)message, strlen(message), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
        printf("Data sent to the Server.\n");

        int n, len;
        len = sizeof(servaddr);
        n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
        buffer[n] = '\0';
        printf("Response from server: %s\n", buffer);

        close(sockfd);
    }

    return 0;
}

