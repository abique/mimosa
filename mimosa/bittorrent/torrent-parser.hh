#ifndef MIMOSA_BITTORRENT_TORRENT_PARSER_HH
# define MIMOSA_BITTORRENT_TORRENT_PARSER_HH

# include <string>

# include "../non-copyable.hh"
# include "../string-ref.hh"
# include "torrent-info.hh"

namespace mimosa
{
  namespace bencode
  {
    class Decoder;
  }

  namespace bittorrent
  {
    class TorrentParser : public NonCopyable
    {
    public:
      enum Error {
        kSucceed,
        kOpenError,
        kInputError,
        kParseError,
      };

      TorrentParser();
      ~TorrentParser();

      bool parseFile(const mimosa::StringRef & path);
      bool parse(stream::Stream::Ptr in);

    private:
      bool parseRoot();
      bool parseInfo();
      bool parseInfoData();
      bool parseInfoFiles();
      bool parseInfoFilesFile();

      Error               error_;
      stream::Stream::Ptr in_;
      bencode::Decoder *  dec_;
      TorrentInfo *       info_;

      bool                got_info_name_;
      bool                got_info_length_;
      bool                got_info_piece_length_;
      bool                got_info_files_;
    };
  }
}

#endif /* !MIMOSA_BITTORRENT_TORRENT_PARSER_HH */
