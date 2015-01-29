/* vim: et sw=4 sts=4 ts=4 : */
#ifndef CATCHAT_CATCHAT_HPP
#define CATCHAT_CATCHAT_HPP

#include <memory>
#include <catchat/common.hpp>

namespace catchat
{

class CATCHAT_API catchat
{
private:
    class impl;
    std::unique_ptr<catchat::impl> _impl;

public:
    catchat();
    ~catchat();

    void dht_port(uint16_t);
    uint16_t dht_port() const;

    void dht_node_id(const std::string&);
    std::string dht_node_id() const;

    void dht_use_ipv4(bool);
    bool dht_use_ipv4() const;

    void dht_use_ipv6(bool);
    bool dht_use_ipv6() const;

    void read_config(const char* filename = nullptr);
    void write_config(const char* filename);

    void dht_start();
    void dht_stop();
};

class CATCHAT_API node
{

public:
    node();
    void run_once();
};

}

#endif /* CATCHAT_CATCHAT_HPP */
