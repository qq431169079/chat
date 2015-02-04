#include <catchat/catchat.hpp>
#include <iostream>
#include <unistd.h>

using std::cout;


int main(int , char **)
{
    catchat::catchat init;
    init.read_config("catchat.cfg");
    init.write_config("catchat.cfg");
    init.dht_start();

    sleep(120);

    init.dht_stop();
    return 0;
}
