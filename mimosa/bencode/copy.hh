#ifndef MIMOSA_BENCODE_COPY_HH
# define MIMOSA_BENCODE_COPY_HH

# include "decoder.hh"
# include "encoder.hh"

namespace mimosa
{
  namespace bencode
  {
    /**
     * @ingroup bencode
     *
     * @brief copy a value from dec to enc
     * @return true if copy succeed, false otherwise
     */
    bool copyValue(Decoder & dec,
                   Encoder & enc,
                   Time      timeout);
  }
}

#endif /* !MIMOSA_BENCODE_COPY_HH */
