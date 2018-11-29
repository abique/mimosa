#pragma once

#include <unistd.h>
#include <fcntl.h>

#include <string>
#include <vector>

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
    inline void fdMap(int parentFd, int childFd) { fds_.push_back(std::make_pair(parentFd, childFd)); }
    bool fdMap(const char *path, int childFd, int flags = O_RDONLY, int mode = 0644);

    /**
     * @brief pipeInput create a pipe, readable for the child, writable for the parent
     * @param childFd the file descriptor to use for the child, for exemple STDIN_FILENO
     * @return the parent file descriptor
     */
    int pipeInput(int childFd);

    /**
     * @brief pipeOutput create a pipe, writable for the child, readable for the parent
     * @param childFd the file descriptor to use for the child, for exemple STDOUT_FILENO
     * @return the parent file descriptor
     */
    int pipeOutput(int childFd);

    std::string dump() const;

  protected:
    std::string path_;
    std::string wd_;
    std::vector<std::string> args_;
    std::vector<std::pair<std::string, std::string> > env_;
    std::vector<std::pair<int, int> > fds_;
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
    pid_t pid_ = -1;
  };
}
