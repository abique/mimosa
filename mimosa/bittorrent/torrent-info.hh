#ifndef MIMOSA_BITTORRENT_TORRENT_INFO_HH
# define MIMOSA_BITTORRENT_TORRENT_INFO_HH

# include <cstdint>
# include <string>
# include <vector>

namespace mimosa
{
  namespace bittorrent
  {
    struct TorrentInfo
    {
      TorrentInfo();
      ~TorrentInfo();

      struct File {
        std::string path_;
        uint64_t    length_;
      };

      struct Tracker {
        std::string url_;
        bool        is_backup_;
      };

      struct Node {
        std::string host_;
        uint16_t    port_;
      };

      struct Sha1 {
        uint8_t bytes_[20];
      };

      std::string          name_;
      std::string          creation_date_;
      uint64_t             length_;
      uint64_t             piece_length_;
      bool                 is_private_;
      std::vector<File>    files_;
      std::vector<Tracker> trackers_;
      std::vector<Node>    nodes_;
      Sha1                 info_hash_;
      Sha1 *               pieces_;
    };
  }
}

#endif /* !MIMOSA_BITTORRENT_TORRENT_INFO_HH */
