#include "lzma-encoder.hh"

namespace mimosa
{
  namespace stream
  {
    LzmaEncoder::LzmaEncoder(Stream::Ptr stream, uint32_t preset, lzma_check check)
      : Filter(stream),
        lzma_(LZMA_STREAM_INIT),
        buffer_(4096)
    {
      if (lzma_easy_encoder(&lzma_, preset, check) != LZMA_OK)
        assert(false);
    }

    int64_t
    LzmaEncoder::write(const char * data, uint64_t nbytes, runtime::Time timeout)
    {
      do {
        lzma_.next_in = (const uint8_t*)data;
        lzma_.avail_in = nbytes;
        lzma_.next_out = (uint8_t*)buffer_.data();
        lzma_.avail_out = buffer_.size();

        lzma_ret ret = lzma_code(&lzma_, LZMA_RUN);
        if (ret != LZMA_OK)
          return -1;

        if (stream_->loopWrite(buffer_.data(), buffer_.size() - lzma_.avail_out, timeout) < 0)
          return -1;
      } while (lzma_.avail_in > 0);

      return nbytes;
    }

    int64_t
    LzmaEncoder::read(char * data, uint64_t nbytes, runtime::Time timeout)
    {
      assert(false);
      return -1;
    }

    bool
    LzmaEncoder::flush(runtime::Time timeout)
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
