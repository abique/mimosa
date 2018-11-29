#pragma once

# include <archive.h>

# include "../stream/stream.hh"
# include "../non-copyable.hh"

namespace mimosa
{
  namespace archive
  {
    /**
     * This class wraps the libarchive write subset.
     * Note that only a subset of the original calls are present;
     * if you need more of them please send a patch.
     */
    class Writer : public stream::Stream
    {
    public:
      MIMOSA_DEF_PTR(Writer);

      inline Writer() : archive_(archive_write_new()) {}
      inline ~Writer() override { archive_write_free(archive_); }

      inline operator struct ::archive * () const { return archive_; }

      int64_t read(char *data, uint64_t nbytes) override;
      int64_t write(const char *data, uint64_t nbytes) override;

      //////////////
      /// Filter ///
      //////////////

      inline int filter(int filter_code) {
        return archive_write_add_filter(archive_, filter_code);
      }
      inline int filter(const std::string & name) {
        return archive_write_add_filter_by_name(archive_, name.c_str());
      }
      inline int filterB64enc() { return archive_write_add_filter_b64encode(archive_); }
      inline int filterBzip2() { return archive_write_add_filter_bzip2(archive_); }
      inline int filterCompress() { return archive_write_add_filter_compress(archive_); }
      inline int filterGzip() { return archive_write_add_filter_gzip(archive_); }
      inline int filterLrzip() { return archive_write_add_filter_lrzip(archive_); }
      inline int filterLzip() { return archive_write_add_filter_lzip(archive_); }
      inline int filterLzma() { return archive_write_add_filter_lzma(archive_); }
      inline int filterLzop() { return archive_write_add_filter_lzop(archive_); }
      inline int filterNone() { return archive_write_add_filter_none(archive_); }
      inline int filterProgram(const std::string & cmd) {
        return archive_write_add_filter_program(archive_, cmd.c_str());
      }
      inline int filterUuenc() { return archive_write_add_filter_uuencode(archive_); }
      inline int filterXz() { return archive_write_add_filter_xz(archive_); }

      //////////////
      /// Format ///
      //////////////

      inline int format(int format_code) {
        return archive_write_set_format(archive_, format_code);
      }
      inline int format(const std::string & name) {
        return archive_write_set_format_by_name(archive_, name.c_str());
      }
      inline int format7zip() { return archive_write_set_format_7zip(archive_); }
      inline int formatArBsd() { return archive_write_set_format_ar_bsd(archive_); }
      inline int formatArSvr4() { return archive_write_set_format_ar_svr4(archive_); }
      inline int formatCpio() { return archive_write_set_format_cpio(archive_); }
      inline int formatCpioNewc() { return archive_write_set_format_cpio_newc(archive_); }
      inline int formatGnutar() { return archive_write_set_format_gnutar(archive_); }
      inline int formatIso9660() { return archive_write_set_format_iso9660(archive_); }
      inline int formatMtree() { return archive_write_set_format_mtree(archive_); }
      inline int formatMtreeClassic() { return archive_write_set_format_mtree_classic(archive_); }
      inline int formatPax() { return archive_write_set_format_pax(archive_); }
      inline int formatPaxRestricted() { return archive_write_set_format_pax_restricted(archive_); }
      inline int formatShar() { return archive_write_set_format_shar(archive_); }
      inline int formatSharDump() { return archive_write_set_format_shar_dump(archive_); }
      inline int formatUstar() { return archive_write_set_format_ustar(archive_); }
      inline int formatV7tar() { return archive_write_set_format_v7tar(archive_); }
      inline int formatXar() { return archive_write_set_format_xar(archive_); }
      inline int formatZip() { return archive_write_set_format_zip(archive_); }

      inline int open(int fd) { return archive_write_open_fd(archive_, fd); }
      inline int open(const std::string & filename) {
        return archive_write_open_filename(archive_, filename.c_str());
      }
      inline int open(FILE *file) {
        return archive_write_open_FILE(archive_, file);
      }
      inline int open(void *buffer, size_t size, size_t * used) {
        return archive_write_open_memory(archive_, buffer, size, used);
      }

      int open(const stream::Stream::Ptr& stream);

      inline void close() override { archive_write_close(archive_); }

    private:
      struct ::archive *archive_;
    };
  }
}

