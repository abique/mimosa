#ifndef MIMOSA_STREAM_HASH_HH
# define MIMOSA_STREAM_HASH_HH

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
    template <typename Ctx,
              void Init(Ctx *),
              void Update(Ctx *, unsigned, const uint8_t *),
              void Digest(Ctx *, unsigned, uint8_t *),
              size_t Len>
    class Hash : public Stream
    {
    public:
      MIMOSA_DEF_PTR(Hash);

      Hash();

      void reset();

      virtual int64_t write(const char * data, uint64_t nbytes);
      virtual int64_t read(char * data, uint64_t nbytes);

      char * digest();

      static inline size_t digestLen() { return Len; }

    private:
      Ctx  ctx_;
      char digest_[Len];
    };

#define MIMOSA_NETTLE_HASH(HASH, hash, Class)                   \
    typedef Hash<struct hash##_ctx,                             \
                 hash##_init,                                   \
                 hash##_update,                                 \
                 hash##_digest,                                 \
                 HASH##_DIGEST_SIZE> Class;                     \
                                                                \
    extern template class Hash<struct hash##_ctx,               \
                               hash##_init,                     \
                               hash##_update,                   \
                               hash##_digest,                   \
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

#undef MIMOSA_NETTLE_HASH
  }
}

#endif /* !MIMOSA_STREAM_HASH_HH */
