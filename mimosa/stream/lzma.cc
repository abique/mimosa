#include <utility>

#include "lzma.hh"

namespace mimosa
{
  namespace stream
  {
    Lzma::Lzma(Stream::Ptr stream)
      : Filter(std::move(stream)),
        lzma_(LZMA_STREAM_INIT),
        buffer_(4096)
    {
    }

    Lzma::~Lzma()
    {
      flush();
      lzma_end(&lzma_);
    }

    int64_t
    Lzma::write(const char * data, uint64_t nbytes)
    {
      lzma_.next_in = (const uint8_t*)data;
      lzma_.avail_in = nbytes;

      do {
        lzma_.next_out = (uint8_t*)buffer_.data();
        lzma_.avail_out = buffer_.size();

        lzma_ret ret = lzma_code(&lzma_, LZMA_RUN);

        if (ret != LZMA_OK && ret != LZMA_STREAM_END)
          return -1;

        if (buffer_.size() > lzma_.avail_out &&
            stream_->loopWrite(buffer_.data(), buffer_.size() - lzma_.avail_out)
            != static_cast<int64_t> (buffer_.size() - lzma_.avail_out))
          return -1;

        if (ret == LZMA_STREAM_END)
          return nbytes - lzma_.avail_in;

      } while (lzma_.avail_in > 0);

      return nbytes;
    }

    int64_t
    Lzma::read(char * /*data*/, uint64_t /*nbytes*/)
    {
      assert(false);
      return -1;
    }

    bool
    Lzma::flush()
    {
      lzma_.next_in = nullptr;
      lzma_.avail_in = 0;

      while (true)
      {
        lzma_.next_out = (uint8_t*)buffer_.data();
        lzma_.avail_out = buffer_.size();
        lzma_ret ret = lzma_code(&lzma_, LZMA_FINISH);

        if (buffer_.size() > lzma_.avail_out &&
            stream_->loopWrite(buffer_.data(), buffer_.size() - lzma_.avail_out) < 0)
          return false;

        if (ret == LZMA_STREAM_END)
          return stream_->flush();

        if (ret != LZMA_OK)
          return false;
      }
    }
  }
}
