#pragma once

#include "json.hh"

namespace mimosa
{
namespace rpc
{
bool httpCall(const std::string               &url,
              const google::protobuf::Message &request,
              google::protobuf::Message       *response);
}
}
