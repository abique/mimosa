#ifndef MIMOSA_NET_SERVER_HH
# define MIMOSA_NET_SERVER_HH

# include <functional>

namespace mimosa
{
  namespace net
  {
    class Server
    {
    public:
      bool bindUnix(const std::string & path);
      void onAccept(Function<void (int fd)>::Ptr handler);

    private:
      Function<void (int fd)>::Ptr handler_;
      int                          fd_;
    };
  }
}

#endif /* !MIMOSA_NET_SERVER_HH */
