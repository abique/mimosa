#include <unistd.h>
#include <fcntl.h>
#include <ctime>

#include <ctime>
#include <sstream>
#include <iomanip>

#include "../options/options.hh"
#include "../time.hh"
#include "../mutex.hh"
#include "../thread.hh"
#include "log.hh"
#include "origin.hh"
#include "../stream/direct-fd-stream.hh"
#include "../stream/zlib-encoder.hh"
#include "../stream/lzma-encoder.hh"
#include "../stream/copy.hh"

namespace mimosa
{
  namespace log
  {
    static bool & ENABLE_COLOR = *options::addSwitch("log", "log-color", "enables colors in log");
    static uint32_t & ROTATION_INTERVAL = *options::addOption<uint32_t>(
      "log", "log-rotation-interval", "enables log file rotation based on interval in seconds", 0);
    static std::string & COMPRESSION = *options::addOption<std::string>(
      "log", "log-compression", "selection the compression method: identity, gzip, xz", "xz");
    static std::string & FILE = *options::addOption<std::string>(
      "log", "log-file", "the path to the log file, it will be passed to strftime(3), ie: my-app-%Y-%m-%d.%H:%M:%S.log", "");

    static const char * const log_colors[] = {
      "\e[0;32m", // debug
      "\e[m",     // info
      "\e[0;33m", // warning
      "\e[0;31m", // error
      "\e[1;31m", // critical
      "\e[1;35m", // fatal
    };

    struct Output
    {
      Output()
        : open_time_(0),
          path_(),
          fd_(STDOUT_FILENO)
      {
      }

      ~Output()
      {
        if (!path_.empty())
          compress(true);
      }

      void check(const struct tm & tm)
      {
        if (FILE.empty())
          return;

        if (open_time_ == 0 ||
            fd_ < 0 ||
            (ROTATION_INTERVAL > 0 &&
             monotonicTimeCoarse() > open_time_ + ROTATION_INTERVAL * second))
          open(tm);
      }

      void open(const struct tm & tm)
      {
        char buffer[PATH_MAX];
        ::strftime(buffer, sizeof (buffer), FILE.c_str(), &tm);
        int fd = ::open(buffer, O_CREAT|O_WRONLY|O_APPEND, 0644);
        if (fd < 0) {
          ::write(STDERR_FILENO, "failed to open log file: ", 25);
          ::write(STDERR_FILENO, buffer, ::strlen(buffer));
          ::write(STDERR_FILENO, "\n", 1);
          return;
        }

        if (!path_.empty())
          compress(false);

        path_ = buffer;
        open_time_ = monotonicTimeCoarse();
        if (::dup2(fd, STDOUT_FILENO) < 0) {
          // keep STDOUT_FILENO open so we can try to re-dup2
          // on next open
          fd_ = fd;
        } else {
          ::close(fd);
          fd_ = STDOUT_FILENO;
        }
      }

      void release()
      {
        if (FILE.empty() || fd_ < 0)
          return;

        ::close(fd_);
        fd_ = -1;
      }

      void compress(bool join)
      {
        if (!::strcasecmp(COMPRESSION.c_str(), "identity"))
          return;

        std::string path(path_);
        Thread thread([path] {
            std::string new_file(path);
            if (!::strcasecmp(COMPRESSION.c_str(), "gzip"))
              new_file.append(".gz");
            else
              new_file.append(".xz");

            int fd = ::open(new_file.c_str(), O_CREAT|O_WRONLY|O_APPEND, 0644);
            if (fd < 0)
              return;
            stream::DirectFdStream::Ptr out_fd_stream = new stream::DirectFdStream(fd, true);
            stream::Stream::Ptr out;

            if (!::strcasecmp(COMPRESSION.c_str(), "gzip"))
              out = new stream::GzipEncoder(out_fd_stream.get());
            else
              out = new stream::LzmaEncoder(out_fd_stream.get());

            fd = ::open(path.c_str(), O_RDONLY, 0644);
            if (fd < 0)
              return;
            stream::DirectFdStream in_fd_stream(fd, true);

            if (stream::copy(in_fd_stream, *out) < 0)
              return;

            if (out->flush())
              ::unlink(path.c_str());
          });
        thread.start();
        if (join)
          thread.join();
      }

      Time        open_time_;
      std::string path_;
      int         fd_;
    };

    static Output output;
    static Mutex  stdout_lock;

    void log(Level level, const Origin * origin, const std::string & msg)
    {
      auto real_time = realTime();
      time_t ts = real_time / second;
      tm tm;
      ::localtime_r(&ts, &tm);
      std::ostringstream os;
      if (ENABLE_COLOR)
        os << log_colors[level];
      os << (tm.tm_year + 1900) << "-"
         << std::setfill('0') << std::setw(2) << (tm.tm_mon + 1) << "-"
         << std::setfill('0') << std::setw(2) << tm.tm_mday << " "
         << std::setfill('0') << std::setw(2) << tm.tm_hour << ":"
         << std::setfill('0') << std::setw(2) << tm.tm_min << ":"
         << std::setfill('0') << std::setw(2) << tm.tm_sec << "."
         << std::setfill('0') << std::setw(4)
         << ((real_time % second) / millisecond) << " ";
      if (origin)
        os << origin->name_ << ": ";
      os << levelName(level) << ": "
         << msg << std::endl;
      if (ENABLE_COLOR)
        os << "\e[m";

      std::string str(os.str());

      Mutex::Locker locker(stdout_lock);
      output.check(tm);
      ::write(output.fd_, str.c_str(), str.size());
    }

    void release()
    {
      Mutex::Locker locker(stdout_lock);
      output.release();
    }
  }
}
