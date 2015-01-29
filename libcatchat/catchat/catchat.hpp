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
};

class CATCHAT_API node
{

public:
    node();
    void run_once();
};

}

#endif /* CATCHAT_CATCHAT_HPP */
