#include "hash.hh"
#include "hash.hxx"

namespace mimosa
{
  namespace stream
  {
    template class Hash<GNUTLS_DIG_MD2, 16>;
    template class Hash<GNUTLS_DIG_MD5, 16>;
    template class Hash<GNUTLS_DIG_SHA1, 20>;
    template class Hash<GNUTLS_DIG_SHA224, 28>;
    template class Hash<GNUTLS_DIG_SHA256, 32>;
    template class Hash<GNUTLS_DIG_SHA384, 48>;
    template class Hash<GNUTLS_DIG_SHA512, 64>;
  }
}
