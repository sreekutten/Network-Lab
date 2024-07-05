#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080

int main() {
    int client_fd;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    char hello[100];

    printf("Enter message from client: ");
    fgets(hello, sizeof(hello), stdin);
    hello[strcspn(hello, "\n")] = 0;

    while (strcmp(hello, "exit") != 0) {
        client_fd = socket(AF_INET, SOCK_STREAM, 0);

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(PORT);

        connect(client_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
        send(client_fd, hello, strlen(hello), 0);

        read(client_fd, buffer, 1024);
        if (strcmp(buffer, "exit") == 0) {
            printf("Server Terminated\n");
            break;
        }
        printf("%s received from server\n", buffer);

        printf("Enter message from client: ");
        fgets(hello, sizeof(hello), stdin);
        hello[strcspn(hello, "\n")] = 0;

        close(client_fd);
    }

    return 0;
}

