#ifndef MIMOSA_PROCESS_HH
# define MIMOSA_PROCESS_HH

# include <unistd.h>

# include <string>
# include <vector>

namespace mimosa
{
  class Process
  {
  public:
    inline void setExecutable(const std::string & path) { path_ = path; }
    inline void addArg(const std::string & arg) { args_.push_back(arg); }

    bool start();
    int wait();

    inline pid_t pid() const { return pid_; }

  private:
    std::string path_;
    std::vector<std::string> args_;
    pid_t pid_;
  };
}

#endif /* !MIMOSA_PROCESS_HH */
