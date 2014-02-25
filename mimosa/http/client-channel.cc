#include "client-channel.hh"

namespace mimosa
{
  namespace http
  {
    ClientChannel::ClientChannel(stream::BufferedStream::Ptr stream)
      : stream_(stream),
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
  }
}
