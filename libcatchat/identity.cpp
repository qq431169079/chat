/* vim: et sw=4 sts=4 ts=4 : */
#include <catchat/identity.hpp>
#include <catchat/catchat.hpp>
#include <catchat/exceptions.hpp>

#include "compat.hpp"
#include "keygen.hpp"

#include <botan/auto_rng.h>

using namespace std;
using namespace Botan;

namespace catchat
{

class identity::impl
{
private:
    bool _started = false;
    identity& _identity;
    catchat& _parent;
    AutoSeeded_RNG _rng;
    unique_ptr<Private_Key> _priv_key;

public:
    impl(const impl&) = delete;

    impl(identity& identity, catchat& parent)
        : _identity(identity), _parent(parent)
    {
    }

    ~impl()
    {
        stop();
    }

    void credentials(const string& username, const string& password)
    {
        keygen kg;
        unique_ptr<Private_Key> priv = kg.generate(username, password);

        if (!priv->check_key(_rng, true)) {
            throw credentials_error("key too weak");
        }

        _priv_key = std::move(priv);
    }

    void start()
    {
        if (_started) {
            return;
        }

        if (nullptr == _priv_key) {
            throw credentials_error("missing credentials");
        }

        _parent.add(&_identity);
        _started = true;
    }

    void stop()
    {
        if (!_started) {
            return;
        }

        _parent.remove(&_identity);
        _started = false;
    }
};

identity::identity(catchat& parent)
    : _impl(make_unique<identity::impl>(*this, parent))
{

}

identity::~identity()
{

}

void identity::credentials(const string& username,
                           const string& password)
{
    _impl->credentials(username, password);
}

void identity::start()
{
    _impl->start();
}

void identity::stop()
{
    _impl->stop();
}

}
