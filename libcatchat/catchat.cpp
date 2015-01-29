/* vim: et sw=4 sts=4 ts=4 : */
#include <botan/botan.h>
#include <catchat/catchat.hpp>
#include <iostream>
#include "compat.hpp"

using namespace std;

namespace catchat
{

class catchat::impl
{
    Botan::LibraryInitializer init;
};

catchat::catchat()
    : _impl(make_unique<catchat::impl>())
{

}

catchat::~catchat()
{

}

node::node()
{

}

void node::run_once()
{
    cout << "Hello World" << endl;
}

}

extern "C"
{

int dht_blacklisted(const struct sockaddr *, int )
{
    return 0;
}

void dht_hash(void *,
                int ,
                const void *,
                int ,
                const void *,
                int ,
                const void *,
                int )
{
}

int dht_random_bytes(void *, size_t)
{
    return 0;
}

}
