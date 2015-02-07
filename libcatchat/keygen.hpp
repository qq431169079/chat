/* vim: et sw=4 sts=4 ts=4 : */
#ifndef CATCHAT_KEYGEN_HPP
#define CATCHAT_KEYGEN_HPP

#include <botan/pbkdf.h>
#include <botan/pk_keys.h>
#include <botan/ec_group.h>
#include <botan/sha2_32.h>
#include <botan/auto_rng.h>
#include <memory>
#include <catchat/common.hpp>

namespace catchat
{

class keygen
{
private:
    Botan::PBKDF *_pbkdf;
    Botan::EC_Group _group;
    Botan::SHA_256 _hash;
    Botan::AutoSeeded_RNG _rng;

public:
    keygen();

    std::unique_ptr<Botan::Private_Key> generate(const std::string&,
                                                 const std::string&);

};

}

#endif /* CATCHAT_KEYGEN_HPP */
