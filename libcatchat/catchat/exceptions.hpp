/* vim: et sw=4 sts=4 ts=4 : */
#ifndef CATCHAT_EXCEPTIONS_HPP
#define CATCHAT_EXCEPTIONS_HPP

#include <catchat/common.hpp>

#include <stdexcept>

class CATCHAT_API communication_error : public std::runtime_error
{
    using std::runtime_error::runtime_error;
};

class CATCHAT_API configuration_error : public std::runtime_error
{
    using std::runtime_error::runtime_error;
};

#endif /* CATCHAT_EXCEPTIONS_HPP */
