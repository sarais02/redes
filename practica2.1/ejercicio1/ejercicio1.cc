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
    int sd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	rc = bind(sd, result->ai_addr, result->ai_addrlen);

	freeaddrinfo(result);
	if(rc==-1){
		std__cerr << "[bind]: " << stderror(errno) << "\n";
		return -1;
	}

	return 0;
}