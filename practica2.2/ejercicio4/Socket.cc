#include <string.h>

#include "Serializable.h"
#include "Socket.h"

Socket::Socket(const char * address, const char * port):sd(-1)
{
    //Construir un socket de tipo AF_INET y SOCK_DGRAM usando getaddrinfo.
    struct addrinfo hints;
    struct addrinfo* result;
    
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family=AF_INET;
    hints.ai_socktype=SOCK_DGRAM;

    int rc =getaddrinfo(address, port, &hints, &result);
    if(rc!=0){
        std::cout<<"ERROR"<< std::endl;
        return;
    }
    //Con el resultado inicializar los miembros sd, sa y sa_len de la clase
    sd=socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if(sd==-1){
        std::cout<<"ERROR creating socket"<< std::endl;
        return;
    }
    sa = (*result->ai_addr);
    sa_len = result->ai_addrlen;
}

int Socket::recv(Serializable &obj, Socket * &sock)
{
    struct sockaddr sa;
    socklen_t sa_len = sizeof(struct sockaddr);

    char buffer[MAX_MESSAGE_SIZE];

    ssize_t bytes = ::recvfrom(sd, buffer, MAX_MESSAGE_SIZE, 0, &sa, &sa_len);

    if ( bytes <= 0 )
    {
        return -1;
    }

    if ( sock != 0 )
    {
        sock = new Socket(&sa, sa_len);
    }

    obj.from_bin(buffer);

    return 0;
}

int Socket::send(Serializable& obj, const Socket& sock)
{
    //Serializar el objeto
    obj.to_bin();
    //Enviar el objeto binario a sock usando el socket sd
    int s=sendto(sd, obj.data(), obj.size(), 0, &sock.sa, sock.sa_len);
    return s;
}

bool operator== (const Socket &s1, const Socket &s2)
{   
    //Comparar los campos sin_family, sin_addr.s_addr y sin_port
    //de la estructura sockaddr_in de los Sockets s1 y s2
    //Retornar false si alguno difiere
    struct sockaddr_in* info1 = (struct sockaddr_in*) &s1.sa;
    struct sockaddr_in* info2 = (struct sockaddr_in*) &s2.sa;
    return info1->sin_family==info2->sin_family && info1->sin_addr.s_addr==info2->sin_addr.s_addr && info1->sin_port==info2->sin_port;
};

std::ostream& operator<<(std::ostream& os, const Socket& s)
{
    char host[NI_MAXHOST];
    char serv[NI_MAXSERV];

    getnameinfo((struct sockaddr *) &(s.sa), s.sa_len, host, NI_MAXHOST, serv,
                NI_MAXSERV, NI_NUMERICHOST);

    os << host << ":" << serv;

    return os;
};

