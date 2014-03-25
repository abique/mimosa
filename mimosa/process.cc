#include <sys/wait.h>

#include "process.hh"

namespace mimosa
{
  bool
  Process::start()
  {
    pid_ = fork();
    if (pid_ < 0)
      return false;

    if (pid_ > 0)
      // in the parent
      return true;

    // in the child
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
