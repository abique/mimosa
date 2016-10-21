#include "../http/client-channel.hh"
#include "http-call.hh"
#include "json.hh"

namespace mimosa
{
namespace rpc
{

bool httpCall(const std::string               &url,
              const google::protobuf::Message &request,
              google::protobuf::Message       *response)
{
    http::ClientChannel cc;
    http::RequestWriter::Ptr rw = new http::RequestWriter(cc);

    /* request header */
    rw->setUrl(url);
    rw->setMethod(http::kMethodPost);
    rw->setProto(1, 1);
    rw->setContentLength(request.ByteSize());
    rw->setContentType("application/json");
    if (!rw->sendRequest())
        return false;

    /* request body (JSON) */
    jsonEncode(rw.get(), request);

    /* decode response */
    auto rr = rw->response();
    if (!rr)
        return false;

    if (response)
        jsonDecode(rr.get(), response);

    return true;
}

}
}
