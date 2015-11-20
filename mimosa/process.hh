#ifndef MIMOSA_PROCESS_HH
# define MIMOSA_PROCESS_HH

# include <unistd.h>

# include <string>
# include <vector>

namespace mimosa
{
  class ProcessConfig
  {
  public:
    inline void setExecutable(const std::string & path) { path_ = path; }
    inline void addArg(const std::string & arg) { args_.push_back(arg); }
    inline void setWorkingDirectory(const std::string & path) { wd_ = path; }
    inline void setEnv(const std::string & key, const std::string & value) {
      env_.push_back(std::make_pair(key, value));
    }

    std::string dump() const;

  protected:
    std::string path_;
    std::string wd_;
    std::vector<std::string> args_;
    std::vector<std::pair<std::string, std::string> > env_;
  };

  class Process : public ProcessConfig
  {
  public:
    Process();
    Process(const ProcessConfig & cfg);

    bool start();
    int wait();

    inline pid_t pid() const { return pid_; }

  private:
    pid_t pid_;
  };
}

#endif /* !MIMOSA_PROCESS_HH */
