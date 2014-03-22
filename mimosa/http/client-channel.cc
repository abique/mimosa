#include "../uri/url.hh"
#include "../net/connect.hh"
#include "../stream/net-fd-stream.hh"
#include "../stream/tls-stream.hh"
#include "client-channel.hh"

namespace mimosa
{
  namespace http
  {
    ClientChannel::ClientChannel()
      : stream_(),
        addr_(nullptr),
        addr_len_(0),
        read_timeout_(0),
        write_timeout_(0),
        is_ssl_(false)
    {
    }

    ClientChannel::~ClientChannel()
    {
    }

    bool
    ClientChannel::connect(const std::string & host, uint16_t port, bool ssl)
    {
      int fd = net::connectToHost(host, port);
      if (fd < 0)
        return false;

      stream::NetFdStream::Ptr net_stream = new stream::NetFdStream(fd, true);
      stream::Stream::Ptr stream(net_stream);
      if (ssl)
        stream = new stream::TlsStream(stream, false);
      stream_ = new stream::BufferedStream(stream);
      return true;
    }

    ResponseReader::Ptr
    ClientChannel::get(const std::string & raw_url)
    {
      uri::Url url;
      if (!url.parse(raw_url, nullptr))
	return nullptr;

      RequestWriter rw(*this);
      rw.setUrl(url);
      rw.setMethod(kMethodGet);
      rw.setProto(1, 1);
      return rw.send();
    }
  }
}
