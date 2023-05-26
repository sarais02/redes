#include <thread>
#include "Server.h"
int main(int argc, char **argv){
    
    Server es(argv[1], argv[2]);
  
    std::thread net_thread([&es](){ es.do_messages(); });
    es.input_thread();
    return 0;
}