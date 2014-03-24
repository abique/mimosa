#ifndef MIMOSA_SMTP_CLIENT_HH
# define MIMOSA_SMTP_CLIENT_HH

# include "../non-copyable.hh"
# include "../stream/net-fd-stream.hh"

namespace mimosa
{
  namespace smtp
  {
    class Mail;

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
      bool from(const std::string & name);
      bool to(const std::string & name);
      bool data(const std::string & data);
      void quit();
      void close();

      bool sendMail(const Mail & mail);

    private:
      stream::NetFdStream::Ptr stream_;
      // stream::BufferedStream::Ptr stream_;
    };
  }
}

#endif /* !MIMOSA_SMTP_CLIENT_HH */
