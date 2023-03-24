#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>


int main(int argc, char *argv[]){
    struct addrinfo hints;
    struct addrinfo* result;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_flags=AI_PASSIVE;
    hints.ai_family=AF_UNSPEC;
    hints.ai_socktype=SOCK_STREAM;

    int rc = getaddrinfo(argv[1], argv[2], &hints, &result);

    if(rc!=0){
        std::cerr<<"[addrinfo]: "<<gai_strerror(rc)<<"\n";
        return -1;
    }
    for(struct addrinfo* i = result; i !=0; i=i->ai_next)
    {
        char host[NI_MAXHOST];
        char server[NI_MAXSERV];
        getnameinfo(i->ai_addr, i->ai_addrlen, host, NI_MAXHOST, server, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
        std::cout << "IP: " << host << " PORT: " << server << "\n";
    }
    freeaddrinfo(result);
	return 0;
}