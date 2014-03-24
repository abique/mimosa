#include <cstring>

#include "client.hh"
#include "mail.hh"
#include "../net/connect.hh"

namespace mimosa
{
  namespace smtp
  {
    Client::Client()
    {
    }

    bool
    Client::connect(const std::string & host, uint16_t port)
    {
      close();

      int fd = net::connectToHost(host, port);
      if (fd < 0)
        return false;

      stream_ = new stream::NetFdStream(fd);
      if (!stream_)
        return false;

      // stream_ = new stream::BufferedStream(net_stream_);
      // if (!stream_)
      //   return false;

      char buffer[1024];
      auto nbytes = stream_->read(buffer, sizeof (buffer));
      if (nbytes < 5 || strncmp("220", buffer, 3)) {
        close();
        return false;
      }

      return stream_;
    }

    void
    Client::close()
    {
      stream_ = nullptr;
    }

    bool
    Client::hello(const std::string & name)
    {
      if (!stream_)
        return false;

      char buffer[1024];
      auto nbytes = snprintf(buffer, sizeof (buffer), "HELO %s\r\n", name.c_str());
      if (stream_->loopWrite(buffer, nbytes) != nbytes) {
        close();
        return false;
      }

      nbytes = stream_->read(buffer, sizeof (buffer));
      if (nbytes < 5 || strncmp("250", buffer, 3)) {
        close();
        return false;
      }

      return true;
    }

    bool
    Client::from(const std::string & name)
    {
      if (!stream_)
        return false;

      char buffer[1024];
      auto nbytes = snprintf(buffer, sizeof (buffer), "MAIL FROM:%s\r\n", name.c_str());
      if (stream_->loopWrite(buffer, nbytes) != nbytes) {
        close();
        return false;
      }

      nbytes = stream_->read(buffer, sizeof (buffer));
      if (nbytes < 5 || strncmp("250", buffer, 3)) {
        close();
        return false;
      }

      return true;
    }

    bool
    Client::to(const std::string & name)
    {
      char buffer[1024];
      auto nbytes = snprintf(buffer, sizeof (buffer), "RCPT TO:%s\r\n", name.c_str());
      if (stream_->loopWrite(buffer, nbytes) != nbytes) {
        close();
        return false;
      }

      nbytes = stream_->read(buffer, sizeof (buffer));
      if (nbytes < 5 || strncmp("250", buffer, 3)) {
        close();
        return false;
      }

      return true;
    }

    bool
    Client::data(const std::string & data)
    {
      if (!stream_)
        return false;

      if (stream_->loopWrite("DATA\r\n", 6) != 6) {
        close();
        return false;
      }

      char buffer[1024];
      auto nbytes = stream_->read(buffer, sizeof (buffer));
      if (nbytes < 5 || strncmp("354", buffer, 3)) {
        close();
        return false;
      }

      struct iovec iov[2];
      iov[0].iov_base = (void*)data.c_str();
      iov[0].iov_len  = data.size();
      iov[1].iov_base = (void*)"\r\n.\r\n";
      iov[1].iov_len  = 5;

      nbytes = stream_->loopWritev(iov, 2);
      if (nbytes != (ssize_t)(iov[0].iov_len + iov[1].iov_len)) {
        close();
        return false;
      }

      nbytes = stream_->read(buffer, sizeof (buffer));
      if (nbytes < 5 || strncmp("250", buffer, 3)) {
        close();
        return false;
      }

      return true;
    }

    void
    Client::quit()
    {
      if (!stream_)
        return;

      stream_->loopWrite("QUIT\r\n", 6);
      close();
    }

    bool
    Client::sendMail(const Mail & mail)
    {
      if (!stream_)
        return false;

      if (!from(mail.from))
        return false;

      for (auto & dest : mail.to)
        if (!to(dest))
          return false;
      for (auto & dest : mail.cc)
        if (!to(dest))
          return false;
      return data(mail.data());
    }
  }
}
