#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <thread>
#include <vector>

void client_handler(int client_sd, const char* host, const char* serv) {
    while (true) {
        char buffer[1500];
        ssize_t bytes = recv(client_sd, buffer, 1499, 0);
        if (bytes == 0) {
            std::cout << "FIN DE CONEXION [" << host << ":" << serv << "]\n";
            break;
        }
        buffer[bytes] = '\0';
        std::cout << "\tMSG: " << buffer;
        send(client_sd, buffer, bytes, 0);
    }
    close(client_sd);
}

int main(int argc, char* argv[]) {
    struct addrinfo hints;
    struct addrinfo* result;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_INET; //ipv4
    hints.ai_socktype = SOCK_STREAM;

    int rc = getaddrinfo(argv[1], argv[2], &hints, &result);

    if (rc != 0) {
        std::cerr << "[addrinfo]: " << gai_strerror(rc) << "\n";
        return -1;
    }

    int sd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    rc = bind(sd, result->ai_addr, result->ai_addrlen);

    if (rc == -1) {
        std::cerr << "[bind]: " << strerror(errno) << "\n";
        return -1;
    }
    freeaddrinfo(result);
    listen(sd, 16);

    std::vector<std::thread> threads;
    while (true) {
        char host[NI_MAXHOST];
        char serv[NI_MAXSERV];
        struct sockaddr client; //vale para ipv4 y ipv6
        socklen_t  clientlen = sizeof(struct sockaddr);
        int client_sd = accept(sd, (struct sockaddr)&client, &clientlen);
        getnameinfo((struct sockaddr)&client, clientlen, host, NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
        std::cout << "NUEVA CONEXION: IP " << host << " [: " << serv << "]\n";
        threads.emplace_back(client_handler, client_sd, host, serv);
        threads.back().detach(); // no esperamos a que termine el hilo
    }

    return 0;
}