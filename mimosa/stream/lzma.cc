#include "lzma.hh"

namespace mimosa
{
  namespace stream
  {
    Lzma::Lzma(Stream::Ptr stream)
      : Filter(stream),
        lzma_(LZMA_STREAM_INIT),
        buffer_(4096)
    {
    }

    Lzma::~Lzma()
    {
      lzma_end(&lzma_);
    }

    int64_t
    Lzma::write(const char * data, uint64_t nbytes, runtime::Time timeout)
    {
      do {
        lzma_.next_in = (const uint8_t*)data;
        lzma_.avail_in = nbytes;
        lzma_.next_out = (uint8_t*)buffer_.data();
        lzma_.avail_out = buffer_.size();

        lzma_ret ret = lzma_code(&lzma_, LZMA_RUN);
        if (ret != LZMA_OK) {
          fprintf(stderr, "%d\n", ret);
          return -1;
        }

        if (stream_->loopWrite(buffer_.data(), buffer_.size() - lzma_.avail_out, timeout) < 0)
          return -1;
      } while (lzma_.avail_in > 0);

      return nbytes;
    }

    int64_t
    Lzma::read(char * data, uint64_t nbytes, runtime::Time timeout)
    {
      assert(false);
      return -1;
    }

    bool
    Lzma::flush(runtime::Time timeout)
    {
      while (true)
      {
        lzma_.next_out = (uint8_t*)buffer_.data();
        lzma_.avail_out = buffer_.size();
        lzma_ret ret = lzma_code(&lzma_, LZMA_FINISH);

        if (stream_->loopWrite(buffer_.data(), buffer_.size() - lzma_.avail_out, timeout) < 0)
          return false;

        if (ret == LZMA_STREAM_END)
          return stream_->flush(timeout);

        if (ret != LZMA_OK)
          return false;
      }
    }
  }
}
