#include "../stream/fd-stream.hh"
#include "../stream/tee-stream.hh"
#include "../stream/hash.hh"
#include "../bencode/decoder.hh"
#include "../bencode/encoder.hh"

#include "torrent-parser.hh"

namespace mimosa
{
  namespace bittorrent
  {
    TorrentParser::TorrentParser()
      : error_(kSucceed),
        dec_(nullptr),
        info_(nullptr)
    {
    }

    TorrentParser::~TorrentParser()
    {
      delete info_;
    }

    bool
    TorrentParser::parseFile(const mimosa::StringRef & path)
    {
      stream::FdStream::Ptr stream = stream::FdStream::openFile(path.data());
      if (!stream) {
        error_ = kOpenError;
        return false;
      }
      return parse(stream);
    }

    bool
    TorrentParser::parse(stream::Stream::Ptr in)
    {
      in_ = in;
      bencode::Decoder dec(in_);
      dec_ = &dec;
      delete info_;
      info_ = new TorrentInfo;

      parseRoot();

      dec_ = nullptr;
      return error_ == kSucceed;
    }

    bool
    TorrentParser::parseRoot()
    {
      bool got_info = false;

      auto token = dec_->pull();
      if (token != bencode::kDict) {
        error_ = kParseError;
        return false;
      }

      while (true) {
        token = dec_->pull();

        switch (token) {
        case bencode::kData:
          // XXX announce
          // XXX nodes

          if (dec_->getData() == "info") {
            if (got_info) {
              error_ = kParseError;
              return false;
            }

            stream::Sha1::Ptr sha1(new stream::Sha1);
            stream::TeeStream::Ptr tee(new stream::TeeStream(in_));
            tee->teeInput(sha1);
            dec_->setInput(tee);

            if (!parseInfo())
              return false;

            dec_->setInput(in_);

            got_info = true;
          } else if (!dec_->eatValue()) {
            error_ = kInputError;
            return false;
          }
          break;

        case bencode::kEnd:
          return true;

        default:
          if (!dec_->eatValue()) {
            error_ = kInputError;
            return false;
          }
          break;
        }
      }
    }

    bool
    TorrentParser::parseInfo()
    {
      auto token = dec_->pull();

      if (token != bencode::kDict) {
        error_ = kParseError;
        return false;
      }

      while (true) {
        token = dec_->pull();
        switch (token) {
        case bencode::kData:
          if (!parseInfoData())
            return false;
          break;

        case bencode::kEnd:
          return true;

        default:
          if (!dec_->eatValue()) {
            error_ = kInputError;
            return false;
          }
          break;
        }
      }
    }

    bool
    TorrentParser::parseInfoData()
    {
      if (dec_->getData() == "name") {
        if (got_info_name_) {
          error_ = kParseError;
          return false;
        }
        got_info_name_ = true;

        auto token = dec_->pull();
        if (token != bencode::kData) {
          error_ = kParseError;
          return false;
        }

        info_->name_ = dec_->getData();
        return true;
      }

      if (dec_->getData() == "length") {
        if (got_info_length_ || got_info_files_) {
          error_ = kParseError;
          return false;
        }
        got_info_length_ = true;

        auto token = dec_->pull();
        if (token != bencode::kInt) {
          error_ = kParseError;
          return false;
        }

        info_->length_ = dec_->getInt();
        return true;
      }

      if (dec_->getData() == "piece length") {
        if (got_info_piece_length_) {
          error_ = kParseError;
          return false;
        }
        got_info_piece_length_ = true;

        auto token = dec_->pull();
        if (token != bencode::kInt) {
          error_ = kParseError;
          return false;
        }

        info_->piece_length_ = dec_->getInt();
        return true;
      }

      if (dec_->getData() == "files") {
        if (got_info_files_ || got_info_length_) {
          error_ = kParseError;
          return false;
        }
        got_info_files_ = true;

        if (!parseInfoFiles())
          return false;
        return true;
      }

      // XXX pieces

      if (!dec_->eatValue()) {
        error_ = kInputError;
        return false;
      }
      return true;
    }

    bool
    TorrentParser::parseInfoFiles()
    {
      auto token = dec_->pull();
      if (token != bencode::kList) {
        error_ = kParseError;
        return false;
      }

      while (true) {
        token = dec_->pull();
        switch (token) {
        case bencode::kDict:
          if (!parseInfoFilesFile())
            return false;
          break;

        case bencode::kEnd:
          return true;

        default:
          if (!dec_->eatValue()) {
            error_ = kInputError;
            return false;
          }
          break;
        }
      }
    }

    bool
    TorrentParser::parseInfoFilesFile()
    {
      TorrentInfo::File file;
      bool got_length = false;
      bool got_path = false;

      while (true) {
        auto token = dec_->pull();
        switch (token) {
        case bencode::kData:
          if (dec_->getData() == "length") {
            if (got_length) {
              error_ = kParseError;
              return false;
            }
            got_length = true;

            token = dec_->pull();
            if (token != bencode::kInt) {
              error_ = kParseError;
              return false;
            }

            file.length_ = dec_->getInt();
          } else if (dec_->getData() == "path") {
            if (got_path) {
              error_ = kParseError;
              return false;
            }
            got_path = true;

            token = dec_->pull();
            if (token != bencode::kData) {
              error_ = kParseError;
              return false;
            }

            file.path_ = dec_->getData();
            if (file.path_.empty()) {
              error_ = kParseError;
              return false;
            }
          } else if (!dec_->eatValue()) {
            error_ = kInputError;
            return false;
          }
          break;

        case bencode::kEnd:
          if (!got_length || !got_path) {
            error_ = kParseError;
            return false;
          }

          info_->files_.push_back(std::move(file));
          return true;

        default:
          if (!dec_->eatValue()) {
            error_ = kInputError;
            return false;
          }
          break;
        }
      }
    }
  }
}
