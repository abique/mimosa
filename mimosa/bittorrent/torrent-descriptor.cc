#include "torrent-descriptor.hh"

namespace mimosa
{
  namespace bittorrent
  {
    TorrentDescriptor::TorrentDescriptor()
      : length_(0),
        piece_length_(0),
        is_private_(false)
    {
    }
  }
}
