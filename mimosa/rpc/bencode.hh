#pragma once

# include <google/protobuf/message.h>
# include <google/protobuf/descriptor.h>

# include "../bencode/encoder.hh"

namespace mimosa
{
  namespace rpc
  {
    void encode(bencode::Encoder & enc,
                const google::protobuf::Message * msg);
  }
}

