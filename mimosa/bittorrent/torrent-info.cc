#include "torrent-info.hh"

namespace mimosa
{
  namespace bittorrent
  {
    TorrentInfo::TorrentInfo()
      : length_(0),
        piece_length_(0),
        is_private_(false),
        pieces_(nullptr)
    {
    }

    TorrentInfo::~TorrentInfo()
    {
      delete[] pieces_;
    }
  }
}
