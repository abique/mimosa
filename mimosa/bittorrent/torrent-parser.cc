#include <memory>

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
        desc_(nullptr),
        got_info_name_(false),
        got_info_length_(false),
        got_info_piece_length_(false),
        got_info_files_(false)
    {
    }

    TorrentParser::~TorrentParser()
    = default;

    bool
    TorrentParser::parseFile(const mimosa::StringRef & path)
    {
      stream::FdStream::Ptr stream = stream::FdStream::openFile(path.data());
      if (!stream) {
        error_ = kOpenError;
        return false;
      }
      return parse(stream.get());
    }

    bool
    TorrentParser::parse(const stream::Stream::Ptr& in)
    {
      in_ = in;
      bencode::Decoder dec(in_);
      dec_ = &dec;
      desc_ = std::make_unique<TorrentDescriptor>();
      parseRoot();

      dec_ = nullptr;
      return error_ == kSucceed;
    }

    bool
    TorrentParser::parseRoot()
    {
      bool got_info = false;

      got_info_name_ = false;
      got_info_length_ = false;
      got_info_piece_length_ = false;
      got_info_files_ = false;

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
          // XXX comment
          // XXX created by

          if (dec_->getData() == "info") {
            if (got_info) {
              error_ = kParseError;
              return false;
            }

            stream::Sha1::Ptr sha1(new stream::Sha1);
            stream::TeeStream::Ptr tee(new stream::TeeStream(in_));
            tee->teeInput(sha1.get());
            dec_->setInput(tee.get());

            if (!parseInfo())
              return false;

            dec_->setInput(in_);

            got_info = true;
            memcpy(desc_->info_hash_.bytes_, sha1->digest(), 20);
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

        desc_->name_ = dec_->getData();
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

        desc_->length_ = dec_->getInt();
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

        desc_->piece_length_ = dec_->getInt();
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
      TorrentDescriptor::File file;
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
            desc_->length_ += file.length_;
          } else if (dec_->getData() == "path") {
            if (got_path) {
              error_ = kParseError;
              return false;
            }
            got_path = true;

            if (!parseInfoFilesFilePath(file))
              return false;
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

          desc_->files_.push_back(std::move(file));
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
    TorrentParser::parseInfoFilesFilePath(TorrentDescriptor::File & file)
    {
      auto token = dec_->pull();
      if (token != bencode::kList) {
        error_ = kParseError;
        return false;
      }

      while (true) {
        token = dec_->pull();

        if (token == bencode::kEnd)
          break;

        if (token != bencode::kData) {
          error_ = kParseError;
          return false;
        }

        if (!file.path_.empty())
          file.path_.append("/", 1);
        file.path_.append(dec_->getData());
      }

      if (file.path_.empty()) {
        error_ = kParseError;
        return false;
      }
      return true;
    }
  }
}
