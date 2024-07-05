#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080
#define MAXLINE 1024

int main() {
    int sockfd;
    char buffer[MAXLINE];
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len;
    int n;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));

    while (1) {
        len = sizeof(cliaddr);

        n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
        buffer[n] = '\0';
        printf("Message from client: %s\n", buffer);

        char response[MAXLINE];
        sprintf(response, "Server received: %s", buffer);

        sendto(sockfd, (const char *)response, strlen(response), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
    }

    return 0;
}

