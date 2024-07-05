#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>

struct pkt {
    int ACK;
    int seqno;
    char data[25];
};

int main() {
    struct pkt sendPkt, recvPkt;
    int sockfd, recvLen, counter = 0;
    struct sockaddr_in addr;
    socklen_t addrSize = sizeof(addr);
    struct timeval timeout = {2, 0}; // 2 second timeout

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Server address and port
    addr.sin_family = AF_INET;
    addr.sin_port = htons(4000);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    while (1) {
        // Prompt user for data input
        printf("Enter data: ");
        scanf("%s", sendPkt.data);

        // Initialize packet fields
        sendPkt.ACK = 0;
        sendPkt.seqno = counter;

        // Send packet
        sendto(sockfd, &sendPkt, sizeof(sendPkt), 0, (const struct sockaddr *)&addr, addrSize);

        // Set timeout for socket receive
        setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout));

        // Wait for ACK
        recvLen = recvfrom(sockfd, &recvPkt, sizeof(recvPkt), 0, (struct sockaddr *)&addr, &addrSize);
        if (recvLen >= 0) {
            printf("ACK received for packet no. %d\n", recvPkt.seqno);
            counter++; // Move to next sequence number
        } else {
            printf("Timeout! Packet will be resent\n");
        }
    }

    close(sockfd);
    return 0;
}

