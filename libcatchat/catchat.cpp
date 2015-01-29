/* vim: et sw=4 sts=4 ts=4 : */
#include <catchat/catchat.hpp>
#include <catchat/exceptions.hpp>
#include "compat.hpp"
#include "sockets.hpp"

extern "C" {
#include "dht.h"
}

#include <botan/botan.h>
#include <botan/sha160.h>
#include <botan/hex.h>

#include <libconfig.h++>

#include <iostream>
#include <cassert>

#define DHT_HASH_LENGTH 20

using namespace std;
using namespace libconfig;

namespace catchat
{

// Global objects used to implement dht methods
static Botan::AutoSeeded_RNG *s_dht_rng = nullptr;
static Botan::SHA_160 *s_dht_hash = nullptr;

class catchat::impl
{
private:
    Botan::LibraryInitializer _init;
    Botan::AutoSeeded_RNG _rng;
    Botan::SHA_160 _sha160;

    Config _config;

    cat_socket _ipv4 = CAT_SOCKET_INVALID;
    cat_socket _ipv6 = CAT_SOCKET_INVALID;

    void generate_node_id(Botan::byte* id, size_t length)
    {
        _rng.randomize(id, length);
    }

    void generate_config()
    {
        // Node ID
        Botan::byte node_id_buffer[DHT_HASH_LENGTH];
        generate_node_id(node_id_buffer, DHT_HASH_LENGTH);

        Setting& node_id = config_dht().add("node_id", Setting::TypeString);
        node_id = Botan::hex_encode(node_id_buffer, DHT_HASH_LENGTH);

        dht_port(12876);
        dht_use_ipv4(true);
        dht_use_ipv6(true);
    }

    Setting& config_dht()
    {
        try {
            return _config.lookup("dht");
        } catch (SettingNotFoundException e) {
            Setting& root = _config.getRoot();
            return root.add("dht", Setting::TypeGroup);
        }
    }

public:
    impl()
    {
        assert(s_dht_rng == nullptr);
        assert(s_dht_hash == nullptr);

        s_dht_rng = &_rng;
        s_dht_hash = &_sha160;
    }

    void dht_node_id(const std::string& node_id)
    {
        if (DHT_HASH_LENGTH*2 != node_id.length()) {
            throw configuration_error("incorrect length for dht.node_id");
        }

        try {
            _config.lookup("dht.node_id") = node_id;
        } catch (SettingNotFoundException& e) {
            config_dht().add("node_id", Setting::TypeString) = node_id;
        }
    }

    std::string dht_node_id() const
    {
        std::string node_id;
        if (!_config.lookupValue("dht.node_id", node_id)) {
            throw configuration_error("missing dht.node_id");
        }

        if (DHT_HASH_LENGTH*2 != node_id.length()) {
            throw configuration_error("dht.node_id must be correct length");
        }

        return node_id;
    }

    uint16_t dht_port() const
    {
        int v;
        if (!_config.lookupValue("dht.port", v)) {
            throw configuration_error("missing dht.port");
        }

        if (v <= 0x00 || v >= 0x10000) {
            throw configuration_error("dht.port out of valid range");
        }

        return v;
    }

    void dht_port(uint16_t port)
    {
        try {
            _config.lookup("dht.port") = port;
        } catch (SettingNotFoundException& e) {
            config_dht().add("port", Setting::TypeInt) = port;
        }
    }

    void dht_use_ipv4(bool v)
    {
        try {
            _config.lookup("dht.use_ipv4") = v;
        } catch (SettingNotFoundException& e) {
            config_dht().add("use_ipv4", Setting::TypeBoolean) = v;
        }
    }

    bool dht_use_ipv4() const
    {
        bool v;
        if (!_config.lookupValue("dht.use_ipv4", v)) {
            throw configuration_error("missing property dht.use_ipv4");
        }

        return v;
    }

    void dht_use_ipv6(bool v)
    {
        try {
            _config.lookup("dht.use_ipv6") = v;
        } catch (SettingNotFoundException& e) {
            config_dht().add("use_ipv6", Setting::TypeBoolean) = v;
        }
    }

    bool dht_use_ipv6() const
    {
        bool v;
        if (!_config.lookupValue("dht.use_ipv6", v)) {
            throw configuration_error("missing property dht.use_ipv6");
        }

        return v;
    }

    void read_config(const char* filename = nullptr)
    {
        // Passing null loads all the defaults
        if (nullptr == filename) {
            generate_config();
            return;
        }

        // Passing a file name will load the settings from that file, unless
        // there's a problem. Then it loads the defaults.
        try {
            _config.readFile(filename);
        } catch (FileIOException& e) {
            generate_config();
        }
    }

    void write_config(const char* filename)
    {
        _config.writeFile(filename);
    }

    void dht_start()
    {
        struct sockaddr_in sin4;
        struct sockaddr_in6 sin6;
        int r;

        Botan::byte node_id[DHT_HASH_LENGTH];
        size_t v = Botan::hex_decode(node_id, dht_node_id());
        assert(v == DHT_HASH_LENGTH);

        if (dht_use_ipv4()) {
            _ipv4 = socket(PF_INET, SOCK_DGRAM, 0);
            if (!CAT_SOCKET_IS_VALID(_ipv4)) {
                throw communication_error("problem creating ipv4 socket");
            }

            memset(&sin4, 0, sizeof(sin4));

            sin4.sin_family = AF_INET;
            sin4.sin_port = htons(dht_port());
            sin4.sin_addr.s_addr = htonl(INADDR_ANY);

            r = bind(_ipv4, (struct sockaddr*) &sin4, sizeof(sin4));
            if (0 > r) {
                cat_socket_close(_ipv4);
                _ipv4 = CAT_SOCKET_INVALID;
                throw communication_error("problem binding ipv4 socket");
            }
        }

        if (dht_use_ipv6()) {
            _ipv6 = socket(PF_INET6, SOCK_DGRAM, 0);
            if (!CAT_SOCKET_IS_VALID(_ipv6)) {
                // Clean up the other socket
                cat_socket_close(_ipv4);
                _ipv4 = CAT_SOCKET_INVALID;
                throw communication_error("problem creating ipv4 socket");
            }

            int on = 1;
            setsockopt(_ipv6, IPPROTO_IPV6, IPV6_V6ONLY, &on, sizeof(on));
            if (r) {
                // Clean up our socket
                cat_socket_close(_ipv6);
                _ipv6 = CAT_SOCKET_INVALID;

                // Clean up the ipv4 socket
                cat_socket_close(_ipv4);
                _ipv4 = CAT_SOCKET_INVALID;

                throw communication_error("unable to put IPv6 socket in V6ONLY mode");
            }

            memset(&sin6, 0, sizeof(sin6));

            sin6.sin6_family = AF_INET6;
            sin6.sin6_port = htons(dht_port());
            // ipv6 wildcard is all zeros, so the memset took care of that.

            r = bind(_ipv6, (struct sockaddr*) &sin6, sizeof(sin6));
            if (0 > r) {
                cat_socket_close(_ipv4);
                _ipv4 = CAT_SOCKET_INVALID;

                cat_socket_close(_ipv6);
                _ipv6 = CAT_SOCKET_INVALID;
                throw communication_error("problem binding ipv6 socket");
            }
        }

        r = dht_init(_ipv4, _ipv6, static_cast<uint8_t*>(node_id), reinterpret_cast<const uint8_t*>("JC\0\0"));
        if (0 > r) {
            cat_socket_close(_ipv4);
            cat_socket_close(_ipv6);
            _ipv4 = CAT_SOCKET_INVALID;
            _ipv6 = CAT_SOCKET_INVALID;
            throw communication_error("could not start dht");
        }
    }

    void dhp_stop()
    {
        dht_uninit();
    }

    ~impl()
    {
        s_dht_hash = nullptr;
        s_dht_rng = nullptr;
    }
};

catchat::catchat()
    : _impl(make_unique<catchat::impl>())
{
}

catchat::~catchat()
{

}

void catchat::dht_start()
{
    _impl->dht_start();
}

void catchat::dht_port(uint16_t p)
{
    _impl->dht_port(p);
}

uint16_t catchat::dht_port() const
{
    return _impl->dht_port();
}

std::string catchat::dht_node_id() const
{
    return _impl->dht_node_id();
}

void catchat::dht_node_id(const std::string& n)
{
    _impl->dht_node_id(n);
}

void catchat::write_config(const char* f)
{
    _impl->write_config(f);
}

void catchat::read_config(const char* f)
{
    _impl->read_config(f);
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

void dht_hash(void * hash_return,
                int hash_size,
                const void * v1,
                int len1,
                const void * v2,
                int len2,
                const void * v3,
                int len3)
{
    assert(catchat::s_dht_hash != nullptr);
    assert(0 >= hash_size);
    assert(catchat::s_dht_hash->output_length() <= static_cast<size_t>(hash_size));

    catchat::s_dht_hash->clear();
    catchat::s_dht_hash->update(static_cast<const Botan::byte*>(v1), len1);
    catchat::s_dht_hash->update(static_cast<const Botan::byte*>(v2), len2);
    catchat::s_dht_hash->update(static_cast<const Botan::byte*>(v3), len3);
    catchat::s_dht_hash->final(static_cast<Botan::byte*>(hash_return));
}

int dht_random_bytes(void * buffer, size_t size)
{
    assert(catchat::s_dht_rng != nullptr);
    catchat::s_dht_rng->randomize(static_cast<Botan::byte*>(buffer), size);
    return 0;
}

}
