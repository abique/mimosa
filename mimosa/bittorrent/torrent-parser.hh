#pragma once

# include <string>
# include <memory>

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

      inline TorrentDescriptor* result() const { return desc_.get(); }
      inline std::unique_ptr<TorrentDescriptor>&& result() { return std::move(desc_); }
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
      bencode::Decoder *dec_ = nullptr;
      std::unique_ptr<TorrentDescriptor> desc_;

      bool                got_info_name_;
      bool                got_info_length_;
      bool                got_info_piece_length_;
      bool                got_info_files_;
    };
  }
}

