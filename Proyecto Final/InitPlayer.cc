#include <thread>
#include <SDL2/SDL.h>
#include "Player.h"
int main(int argc, char **argv)
{
   
    Player ec(argv[1], argv[2], argv[3]);

    ec.login();

    std::thread net_thread([&ec](){ ec.net_thread(); });
    std::thread net_thread2([&ec](){ ec.bucleVentana(); });
    ec.input_thread();
    
    return 0;
}