#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

struct pkt {
    int ACK;
    int seqno;
    char data[25];
};

int main() {
    struct pkt recvPkt, sendPkt;
    int sockfd, recvLen, drop;
    struct sockaddr_in addr;
    socklen_t addrSize = sizeof(addr);

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Bind to local address and port
    addr.sin_family = AF_INET;
    addr.sin_port = htons(4000);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (const struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port 4000...\n");

    while (1) {
        // Receive packet
        recvLen = recvfrom(sockfd, &recvPkt, sizeof(recvPkt), 0, (struct sockaddr *)&addr, &addrSize);
        if (recvLen >= 0) {
            drop = rand() % 8; // Simulate packet drop or ACK drop (0-7)

            if (drop <= 1) {
                printf("Packet with no. %d received but no ACK will be sent (simulating lost ACK)\n", recvPkt.seqno);
            } else if (drop <= 2) {
                printf("Packet with no. %d received but will be dropped (simulating lost packet)\n", recvPkt.seqno);
            } else {
                printf("Packet with no. %d received, sending ACK\n", recvPkt.seqno);
                sendPkt.ACK = 1;
                sendPkt.seqno = recvPkt.seqno;
                sendto(sockfd, &sendPkt, sizeof(sendPkt), 0, (const struct sockaddr *)&addr, addrSize);
            }
        } else {
            perror("Error receiving packet");
        }
    }

    close(sockfd);
    return 0;
}

