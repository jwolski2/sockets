#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define LISTEN_ADDR "0.0.0.0"
#define LISTEN_PORT 5559
#define MAX_CONNS 1000

int main() {
    // Create socket.
    int sock = socket(AF_INET, SOCK_STREAM, 6);
    if (sock == -1) {
        perror("socket failed");
        return -1;
    }

    // Set SO_REUSEADDR to ensure we can bind if connections are still in
    // TIME_WAIT from last run.
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) == -1) {
        perror("setsockopt failed");
        return -1;
    }

    // Bind socket to addr.
    struct sockaddr_in addr = {
        .sin_family = 0,
        .sin_port = htons(LISTEN_PORT),
        .sin_zero = {0},
    };

    if (inet_pton(AF_INET, LISTEN_ADDR, &addr.sin_addr) == -1) {
        perror("inet_pton failed");
        return -1;
    }

    if (bind(sock, (const struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("bind failed");
        return -1;
    }

    // Start listening.
    if (listen(sock, 1) == -1) {
        perror("listen failed");
        return -1;
    }

    //TODO Uncomment when debug mode added.
    //printf("listening on port %d...\n", LISTEN_PORT);

    // Accept connections until MAX_CONNS.
    int accepted_conns = 0;
    while (accepted_conns < MAX_CONNS) {
        // Accept a connection.
        struct sockaddr_in conn;
        memset(&conn, 0, sizeof(conn));
        socklen_t connlen  = sizeof(conn);
        int connfd = accept(sock, (struct sockaddr *)&conn, &connlen);
        if (connfd == -1) {
            perror("accept failed");
            return -1;
        }

        // Print host/port of accepted connection.
        char addr_str[INET_ADDRSTRLEN];
        if (inet_ntop(AF_INET, &conn.sin_addr.s_addr, addr_str, INET_ADDRSTRLEN) == NULL) {
            perror("inet_ntop failed");
            return -1;
        }

        ++accepted_conns;

        // TODO Uncomment when debug mode added.
        //printf("accepted connection %d from %s:%d\n", accepted_conns, addr_str, conn.sin_port);

        // Close connection immediately.
        close(connfd);
    }

    // Close listening socket.
    close(sock);
}
