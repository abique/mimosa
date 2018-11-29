#include "preproc.hh"

#ifdef MIMOSA_WIN
# include <windows.h>
#endif

#ifdef MIMOSA_UNIX
# include <sys/wait.h>
#endif

#include "process.hh"
#include "log/log.hh"

namespace mimosa
{
  std::string
  ProcessConfig::dump() const
  {
    std::ostringstream os;
    if (!wd_.empty())
      os << "cd " << wd_ << "; ";
    os << "[" << path_ << "]";
    for (const auto& arg : args_)
        os << " \"" << arg << "\"";
    return os.str();
  }

  bool
  ProcessConfig::fdMap(const char *path, int childFd, int flags, int mode)
  {
    int fd = ::open(path, flags, mode);
    if (fd < 0)
      return false;

    fdMap(fd, childFd);
    return true;
  }

  int
  ProcessConfig::pipeInput(int childFd)
  {
    int fds[2];

    if (::pipe(fds))
      return -1;

    fdMap(fds[0], childFd);
    return fds[1];
  }

  int
  ProcessConfig::pipeOutput(int childFd)
  {
    int fds[2];

    if (::pipe(fds))
      return -1;

    fdMap(fds[1], childFd);
    return fds[0];
  }

  Process::Process()
  = default;

  Process::Process(const ProcessConfig & cfg)
    : ProcessConfig(cfg)
  {
  }

  bool
  Process::start()
  {
    pid_ = fork();
    if (pid_ < 0)
      return false;

    if (pid_ > 0) {
      for (auto &fds : fds_)
        ::close(fds.first);

      // in the parent
      return true;
    }

    // set working directory
    if (!wd_.empty() && chdir(wd_.c_str())) {
      mimosa::log::error("chdir(%s) failed: %s", strerror(errno));
      exit(1);
    }

    //////////////////
    // in the child //
    //////////////////

    // setenv
    for (auto & kv : env_)
	{
      setenv(kv.first.c_str(), kv.second.c_str(), true);
	}

    for (auto &fds : fds_) {
      ::dup2(fds.first, fds.second);
      ::close(fds.first);
    }

    // execv
    const char *args[args_.size() + 1];
    for (size_t i = 0; i < args_.size(); ++i)
      args[i] = args_[i].c_str();
    args[args_.size()] = nullptr;
    execv(path_.c_str(), (char **)args);
    exit(1);
    return false;
  }

  int
  Process::wait()
  {
    int status = 0;

    if (waitpid(pid_, &status, 0) < 0)
      return -1;
    return WEXITSTATUS(status);
  }
}
