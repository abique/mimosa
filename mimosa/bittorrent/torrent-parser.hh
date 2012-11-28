#ifndef MIMOSA_BITTORRENT_TORRENT_PARSER_HH
# define MIMOSA_BITTORRENT_TORRENT_PARSER_HH

# include <string>

# include "../string-ref.hh"
# include "torrent-info.hh"

namespace mimosa
{
  namespace bittorrent
  {
    class TorrentParser
    {
    public:
      bool parseFile(const mimosa::StringRef & path);

    private:
      std::string parse_error_;
      TorrentInfo torrent_info_;
    };
  }
}

#endif /* !MIMOSA_BITTORRENT_TORRENT_PARSER_HH */
