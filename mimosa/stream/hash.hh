#pragma once

# include <nettle/md2.h>
# include <nettle/md5.h>
# include <nettle/sha1.h>
# include <nettle/sha2.h>
# include <nettle/sha3.h>

# include "stream.hh"

namespace mimosa
{
  namespace stream
  {

#define MIMOSA_NETTLE_HASH(HASH, hash, Class)                           \
    class Class : public Stream                                         \
    {                                                                   \
    public:                                                             \
      MIMOSA_DEF_PTR(Class);                                            \
                                                                        \
      Class() { reset(); }                                              \
                                                                        \
      void reset() { hash##_init(&ctx_); }                              \
                                                                        \
      virtual int64_t write(const char *data, uint64_t nbytes)          \
      {                                                                 \
        hash##_update(&ctx_, nbytes, (const uint8_t *)data);            \
        return nbytes;                                                  \
      }                                                                 \
                                                                        \
      virtual int64_t read(char *, uint64_t)                            \
      {                                                                 \
        assert(false && "invalid operation");                           \
        errno = EINVAL;                                                 \
        return -1;                                                      \
      }                                                                 \
                                                                        \
      char * digest()                                                   \
      {                                                                 \
        hash##_digest(&ctx_, sizeof (digest_), (uint8_t *)digest_);     \
        return digest_;                                                 \
      }                                                                 \
                                                                        \
      static inline size_t digestLen() { return HASH##_DIGEST_SIZE; }   \
                                                                        \
    private:                                                            \
      hash##_ctx  ctx_;                                                 \
      char        digest_[HASH##_DIGEST_SIZE];                          \
    };

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

#undef MIMOSA_NETTLE_HASH
  }
  }

