#ifndef MIMOSA_RPC_JSON_HH
# define MIMOSA_RPC_JSON_HH

# include <google/protobuf/message.h>
# include <google/protobuf/descriptor.h>

# include "../json/encoder.hh"

namespace mimosa
{
  namespace rpc
  {
    void encode(json::Encoder & enc,
                const google::protobuf::Message * msg);
  }
}

#endif /* !MIMOSA_RPC_JSON_HH */
