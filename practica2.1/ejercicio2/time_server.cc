#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#include <string>

int main(int argc, char *argv[]){
  struct addrinfo hints;
	struct addrinfo *result;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_INET; //ipv4
	hints.ai_socktype = SOCK_DGRAM;

	int rc = getaddrinfo(argv[1], argv[2], &hints, &result);

	if(rc!=0){
		std::cerr << "[addrinfo]: " << gai_strerror(rc) << "\n";
		return -1;
	}

	int sd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	rc = bind(sd, result->ai_addr, result->ai_addrlen);

	freeaddrinfo(result);
	if(rc==-1){
		std::cerr << "[bind]: " << strerror(errno) << "\n";
		return -1;
	}
    while(true){
		char buffer[1500];
		char host[NI_MAXHOST];
		char serv[NI_MAXSERV];
		struct sockaddr client; //vale para ipv4 y ipv6
		socklen_t  clientlen = sizeof(struct sockaddr);
		ssize_t bytes = recvfrom(sd, buffer, 1499, 0, (struct sockaddr*)&client, &clientlen);
		buffer[bytes] = '\0';
		getnameinfo((struct sockaddr*)&client, clientlen, host, NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
        std::cout << "IP: " << host << " [: " << serv << "]\n";
		std::cout << "\tMSG: " << buffer;
		
        int num=atoi(buffer)+1;
		std::string s=std::to_string(num);
		sendto(sd, (void*) s.c_str(), s.length(), 0, (struct sockaddr*)&client, clientlen);

	}
	return 0;
}