#include <catchat/catchat.hpp>
#include <iostream>
using std::cout;


int main(int , char **)
{
    catchat::catchat init;
    init.read_config("catchat.cfg");
    init.write_config("catchat.cfg");
    init.dht_start();
    return 0;
}
