/* vim: et sw=4 sts=4 ts=4 : */
#ifndef CATCHAT_SOCKETS_HPP
#define CATCHAT_SOCKETS_HPP

#ifdef __unix__
#   include <unistd.h>
#   include <sys/types.h>
#   include <sys/socket.h>
#   include <netinet/in.h>
namespace catchat
{
    typedef int cat_socket;
}
#   define cat_socket_close close
#   define CAT_SOCKET_INVALID (-1)
#   define CAT_SOCKET_IS_VALID(x) ((x) >= 0)

#else
#   error No socket wrapper exists for your platform yet.
#endif


#endif /* CATCHAT_SOCKETS_HPP */
