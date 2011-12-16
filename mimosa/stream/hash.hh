#ifndef MIMOSA_STREAM_HASH_HH
# define MIMOSA_STREAM_HASH_HH

# include <gnutls/gnutls.h>
# include <gnutls/crypto.h>

# include "stream.hh"

namespace mimosa
{
  namespace stream
  {
    template <gnutls_digest_algorithm_t Algo, size_t Len>
    class Hash : public Stream
    {
    public:
      MIMOSA_DEF_PTR(Hash);

      Hash();
      ~Hash();

      inline operator bool () { return handle_; }

      /// could fail (oom for exemple)
      bool reset();

      virtual int64_t write(const char * data, uint64_t nbytes, runtime::Time timeout = 0);
      virtual int64_t read(char * data, uint64_t nbytes, runtime::Time timeout = 0);

      char * digest();

      static inline size_t digestLen() { return Len; }
      static inline gnutls_digest_algorithm_t digestType() { return Algo; }

    private:
      gnutls_hash_hd_t handle_;
      char             digest_[Len];
    };

# if 0 // not supported by gnutls
    typedef Hash<GNUTLS_DIG_RMD160, 20> Rmd160;
# endif
    typedef Hash<GNUTLS_DIG_MD2, 16> Md2;
    typedef Hash<GNUTLS_DIG_MD5, 16> Md5;
    typedef Hash<GNUTLS_DIG_SHA1, 20> Sha1;
    typedef Hash<GNUTLS_DIG_SHA224, 28> Sha224;
    typedef Hash<GNUTLS_DIG_SHA256, 32> Sha256;
    typedef Hash<GNUTLS_DIG_SHA384, 48> Sha384;
    typedef Hash<GNUTLS_DIG_SHA512, 64> Sha512;

    extern template class Hash<GNUTLS_DIG_MD2, 16>;
    extern template class Hash<GNUTLS_DIG_MD5, 16>;
    extern template class Hash<GNUTLS_DIG_SHA1, 20>;
    extern template class Hash<GNUTLS_DIG_SHA224, 28>;
    extern template class Hash<GNUTLS_DIG_SHA256, 32>;
    extern template class Hash<GNUTLS_DIG_SHA384, 48>;
    extern template class Hash<GNUTLS_DIG_SHA512, 64>;
  }
}

#endif /* !MIMOSA_STREAM_HASH_HH */
