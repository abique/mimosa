#include <gtest/gtest.h>

#include "../hash.hh"
#include "../string-stream.hh"
#include "../base16-encoder.hh"

namespace mimosa
{
  namespace stream
  {
    namespace
    {
#define HASH_TEST(Type, Input, Output)                                  \
      TEST(Hash##Type, Simple)                                          \
      {                                                                 \
        ASSERT_EQ(Type::digestLen(),                                    \
                  gnutls_hash_get_len(Type::digestType()));             \
                                                                        \
        Type::Ptr hash = new Type;                                      \
        hash->write(Input, sizeof (Input) - 1);                         \
                                                                        \
        ASSERT_EQ((bool)*hash, true);                                   \
                                                                        \
        StringStream::Ptr str = new StringStream;                       \
        Base16Encoder::Ptr filter = new Base16Encoder(str.get());       \
        filter->write(hash->digest(), Type::digestLen());               \
        ASSERT_EQ(str->str(), Output);                                  \
      }

#if 0 // not supported by gnutls
      HASH_TEST(Rmd160, "bonjour", "f02368945726d5fc2a14eb576f7276c0")
#endif
      HASH_TEST(Md5, "bonjour", "f02368945726d5fc2a14eb576f7276c0")
      HASH_TEST(Sha1, "bonjour", "1f71e0f4ac9b47cd93bf269e4017abaab9d3bd63")
      HASH_TEST(Sha224, "bonjour", "f8ec9284e160c5afe456b48702e08a14095ab37bca36808e10a22f71")
      HASH_TEST(Sha256, "bonjour", "2cb4b1431b84ec15d35ed83bb927e27e8967d75f4bcd9cc4b25c8d879ae23e18")
      HASH_TEST(Sha384, "bonjour", "c7e2d3a1a600319151a62b9abfcecfbeab4ca388375a1498dc6d7630c67e349140103c150815cd027447db5d117e7e04")
      HASH_TEST(Sha512, "bonjour", "3041edbcdd46190c0acc504ed195f8a90129efcab173a7b9ac4646b92d04cc80005acaa3554f4b1df839eacadc2491cb623bf3aa6f9eb44f6ea8ca005821d25d")
    }
  }
}
