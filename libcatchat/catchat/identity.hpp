/* vim: et sw=4 sts=4 ts=4 : */
#ifndef CATCHAT_IDENTITY_HPP
#define CATCHAT_IDENTITY_HPP

#include <catchat/common.hpp>

#include <memory>

namespace catchat
{

class CATCHAT_API catchat;

class CATCHAT_API identity
{
private:
    class impl;
    std::unique_ptr<impl> _impl;

public:
    identity(catchat& parent);
    ~identity();

    void credentials(const std::string& username, const std::string& password);
    void start();
    void stop();
};

}
#endif /* CATCHAT_IDENTITY_HPP */
