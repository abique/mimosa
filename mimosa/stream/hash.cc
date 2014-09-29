#include "hash.hh"
#include "hash.hxx"

namespace mimosa
{
  namespace stream
  {
#define MIMOSA_NETTLE_HASH(HASH, hash, Class)   \
    template class Hash<struct hash##_ctx,      \
                        hash##_init,            \
                        hash##_update,          \
                        hash##_digest,          \
                        HASH##_DIGEST_SIZE>;


    MIMOSA_NETTLE_HASH(MD2, md2, Md2);
    MIMOSA_NETTLE_HASH(MD5, md5, Md5);
    MIMOSA_NETTLE_HASH(SHA1, sha1, Sha1);
    MIMOSA_NETTLE_HASH(SHA224, sha224, Sha224);
    MIMOSA_NETTLE_HASH(SHA256, sha256, Sha256);
    MIMOSA_NETTLE_HASH(SHA384, sha384, Sha384);
    MIMOSA_NETTLE_HASH(SHA512, sha512, Sha512);
    MIMOSA_NETTLE_HASH(SHA3_224, sha3_224, Sha3_224);
    MIMOSA_NETTLE_HASH(SHA3_256, sha3_256, Sha3_256);
    MIMOSA_NETTLE_HASH(SHA3_384, sha3_384, Sha3_384);
    MIMOSA_NETTLE_HASH(SHA3_512, sha3_512, Sha3_512);
  }
}
