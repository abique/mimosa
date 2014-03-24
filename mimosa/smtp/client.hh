#ifndef MIMOSA_SMTP_CLIENT_HH
# define MIMOSA_SMTP_CLIENT_HH

# include "../non-copyable.hh"
# include "../stream/net-fd-stream.hh"

namespace mimosa
{
  namespace smtp
  {
    /**
     * Simple smtp client.
     * https://tools.ietf.org/html/rfc5321
     */
    class Client : private NonCopyable
    {
    public:
      Client();

      bool connect(const std::string & host, uint16_t port);
      bool hello(const std::string & name);
      void close();

    private:
      stream::NetFdStream::Ptr net_stream_;
      stream::BufferedStream::Ptr stream_;
    };
  }
}

#endif /* !MIMOSA_SMTP_CLIENT_HH */
