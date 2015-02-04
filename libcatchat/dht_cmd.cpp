/* vim: et sw=4 sts=4 ts=4 : */
#include "dht_cmd.hpp"
extern "C"
{
#include "dht.h"
}
#include "compat.hpp"

#include <cstring>
#include <iostream>

using namespace std;

namespace catchat
{

//
// dht_cmd_stop
//
dht_cmd_stop::dht_cmd_stop(ev::loop_ref loop)
    : _loop(loop)
{

}

void dht_cmd_stop::invoke()
{
    _loop.break_loop();
}

//
// dht_cmd_ping_node
//
dht_cmd_ping_node::dht_cmd_ping_node(struct sockaddr* addr, size_t size)
    : _data(make_unique<char[]>(size)), _size(size)
{
    std::memcpy(_data.get(), addr, size);
}

void dht_cmd_ping_node::invoke()
{
    cout << "Ping node." << endl;
    dht_ping_node((struct ::sockaddr*)_data.get(), _size);
}

}

