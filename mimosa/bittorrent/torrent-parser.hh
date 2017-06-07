#pragma once

# include <string>

# include "../non-copyable.hh"
# include "../string-ref.hh"
# include "torrent-descriptor.hh"

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

      /** the reference permits to swap the TorrentDescriptor result */
      inline TorrentDescriptor *& result() { return desc_; }
      inline TorrentDescriptor * result() const { return desc_; }
      inline Error error() const { return error_; }

    private:
      bool parseRoot();
      bool parseInfo();
      bool parseInfoData();
      bool parseInfoFiles();
      bool parseInfoFilesFile();
      bool parseInfoFilesFilePath(TorrentDescriptor::File & file);

      Error               error_;
      stream::Stream::Ptr in_;
      bencode::Decoder *  dec_;
      TorrentDescriptor * desc_;

      bool                got_info_name_;
      bool                got_info_length_;
      bool                got_info_piece_length_;
      bool                got_info_files_;
    };
  }
}

