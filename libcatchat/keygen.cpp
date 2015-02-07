/* vim: et sw=4 sts=4 ts=4 : */
#include "keygen.hpp"
#include "compat.hpp"

#include <stdexcept>
#include <botan/botan.h>
#include <botan/ecdsa.h>

using namespace Botan;

namespace catchat
{

keygen::keygen()
    : _pbkdf(get_pbkdf("PBKDF2(SHA-256)")),
      _group("secp256r1")
{

}

std::unique_ptr<Private_Key> keygen::generate(const std::string& user,
                                              const std::string& pass)
{
    // Derive the salt
    _hash.clear();
    _hash.update(user);
    SecureVector<byte> salt_vec = _hash.final();

    // Derive the private key from the password
    OctetString pkey_string = _pbkdf->derive_key(
            32,
            pass,
            salt_vec,
            salt_vec.size(),
            50000);

    BigInt x(pkey_string.bits_of(), pkey_string.length());

    // Create the public/private pair
    return make_unique<ECDSA_PrivateKey>(_rng, _group, x);
}

}
