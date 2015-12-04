#pragma once

#include <archive.h>

#include "../stream/stream.hh"
#include "../stream/buffer.hh"
#include "../non-copyable.hh"
#include "entry.hh"

namespace mimosa
{
  namespace archive
  {
    class Reader : public stream::Stream
    {
    public:
      MIMOSA_DEF_PTR(Reader);

      inline Reader() : archive_(archive_read_new()) {}
      inline ~Reader() { archive_read_free(archive_); }

      inline operator struct ::archive *() const { return archive_; }

      int open(stream::Stream::Ptr input);

      int nextHeader(Entry &e);

      int64_t read(char *data, uint64_t nbytes) override;
      int64_t write(const char *data, uint64_t nbytes) override;

      //////////////
      /// Filter ///
      //////////////

      inline int filterBzip2() { return archive_read_support_filter_bzip2(archive_); }
      inline int filterCompress() { return archive_read_support_filter_compress(archive_); }
      inline int filterGzip() { return archive_read_support_filter_gzip(archive_); }
      inline int filterLrzip() { return archive_read_support_filter_lrzip(archive_); }
      inline int filterLzip() { return archive_read_support_filter_lzip(archive_); }
      inline int filterLzma() { return archive_read_support_filter_lzma(archive_); }
      inline int filterLzop() { return archive_read_support_filter_lzop(archive_); }
      inline int filterNone() { return archive_read_support_filter_none(archive_); }
      inline int filterProgram(const std::string & cmd) {
        return archive_read_support_filter_program(archive_, cmd.c_str());
      }
      inline int filterXz() { return archive_read_support_filter_xz(archive_); }

      //////////////
      /// Format ///
      //////////////

      inline int format7zip() { return archive_read_support_format_7zip(archive_); }
      inline int formatAr() { return archive_read_support_format_ar(archive_); }
      inline int formatCpio() { return archive_read_support_format_cpio(archive_); }
      inline int formatGnutar() { return archive_read_support_format_gnutar(archive_); }
      inline int formatIso9660() { return archive_read_support_format_iso9660(archive_); }
      inline int formatMtree() { return archive_read_support_format_mtree(archive_); }
      inline int formatTar() { return archive_read_support_format_tar(archive_); }
      inline int formatXar() { return archive_read_support_format_xar(archive_); }
      inline int formatZip() { return archive_read_support_format_zip(archive_); }

    private:
      static int openCb(struct archive *, void *_client_data);
      static ssize_t readCb(struct archive *,
                            void *_client_data, const void **_buffer);
      static int closeCb(struct archive *, void *_client_data);

      void close() override { archive_read_close(archive_); }

      struct ::archive *archive_;
      stream::Stream::Ptr stream_;
      stream::Buffer::Ptr buffer_;
    };
  }
}
