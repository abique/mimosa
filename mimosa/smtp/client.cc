#include <cstring>

#include "client.hh"
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

      net_stream_ = new stream::NetFdStream(fd);
      if (!net_stream_)
        return false;

      stream_ = new stream::BufferedStream(net_stream_);
      if (!stream_)
        return false;

      char buffer[512];
      auto nbytes = stream_->read(buffer, sizeof (buffer));
      if (nbytes < 5 || strncmp("200", buffer, 3)) {
        close();
        return true;
      }

      return stream_;
    }

    bool
    Client::hello(const std::string & name)
    {
      char buffer[256];
      auto nbytes = snprintf(buffer, sizeof (buffer), "HELO %s\r\n", name.c_str());
      if (stream_->loopWrite(buffer, nbytes) != nbytes) {
        close();
        return false;
      }

      return true;
    }
  }
}
