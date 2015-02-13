/* vim: et sw=4 sts=4 ts=4 : */
#ifndef CATCHAT_EXCEPTIONS_HPP
#define CATCHAT_EXCEPTIONS_HPP

#include <catchat/common.hpp>

#include <stdexcept>

class CATCHAT_API credentials_error : public std::runtime_error
{
public:
    credentials_error(const std::string& what)
        : std::runtime_error(what) {}
    credentials_error(const char* what)
        : std::runtime_error(what) {}
};

class CATCHAT_API communication_error : public std::runtime_error
{
public:
    communication_error(const std::string& what)
        : std::runtime_error(what) {}
    communication_error(const char* what)
        : std::runtime_error(what) {}
};

class CATCHAT_API configuration_error : public std::runtime_error
{
public:
    configuration_error(const std::string& what)
        : std::runtime_error(what) {}
    configuration_error(const char* what)
        : std::runtime_error(what) {}
};

#endif /* CATCHAT_EXCEPTIONS_HPP */
