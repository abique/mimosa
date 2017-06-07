#pragma once

# include <google/protobuf/message.h>
# include <google/protobuf/descriptor.h>

# include "../json/encoder.hh"
# include "../json/decoder.hh"

namespace mimosa
{
  namespace rpc
  {
    void jsonEncode(json::Encoder &                   enc,
                    const google::protobuf::Message & msg);

    void jsonEncode(stream::Stream::Ptr               output,
                    const google::protobuf::Message & msg);

    void jsonDecode(json::Decoder &             dec,
                    google::protobuf::Message * msg);

    void jsonDecode(stream::Stream::Ptr         input,
                    google::protobuf::Message * msg);
  }
}

