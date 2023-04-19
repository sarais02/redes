#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(int argc, char argv[]) {

    int c_socket;
    struct sockaddr_in s_address;
    char buffer[BUFFER_SIZE];

    if (argc != 4) {
        printf("Usando: %s puerto\n", argv[0]);
        return 0;
    }

    c_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (c_socket < 0) {
        perror("Error socket");
        return 0;
    }

    s_address.sin_port = htons(atoi(argv[2]));
    s_address.sin_addr.s_addr = inet_addr(argv[1]);
    s_address.sin_family = AF_INET;


    if (connect(c_socket, (struct sockaddr)&s_address, sizeof(s_address)) < 0) {
        perror("Error servidor");
        return 0;
    }

    if (send(c_socket, argv[3], strlen(argv[3]), 0) < 0) {
        perror("Fallo envio al servidor");
        return 0;
    }

    if (recv(c_socket, buffer, BUFFER_SIZE, 0) < 0) {
        perror("Fallo recibir respuesta");
        return 0;
    }
    printf("%s", buffer);

    close(c_socket);

    return 0;
}