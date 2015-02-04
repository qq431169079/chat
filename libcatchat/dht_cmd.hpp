/* vim: et sw=4 sts=4 ts=4 : */
#ifndef CATCHAT_DHT_CMD_HPP
#define CATCHAT_DHT_CMD_HPP
#include <ev++.h>
#include <sys/socket.h>

#include <memory>

namespace catchat
{

class dht_cmd
{
public:
    virtual ~dht_cmd() = default;
    virtual void invoke() =0;

protected:
    dht_cmd() = default;
};

class dht_cmd_stop : public dht_cmd
{
private:
    ev::loop_ref _loop;

public:
    dht_cmd_stop(ev::loop_ref loop);
    virtual void invoke() override;
};

class dht_cmd_ping_node : public dht_cmd
{
private:
    std::unique_ptr<char[]> _data;
    size_t _size;

public:
    dht_cmd_ping_node(struct ::sockaddr*, size_t);

    virtual void invoke() override;
};

}

#endif /* CATCHAT_DHT_CMD_HPP */
