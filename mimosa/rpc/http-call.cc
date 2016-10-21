#include "../stream/string-stream.hh"
#include "../stream/copy.hh"
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

    /* request body (JSON) */
    stream::StringStream::Ptr data = new stream::StringStream;
    jsonEncode(data.get(), request);

    /* request header */
    rw->setUrl(url);
    rw->setMethod(http::kMethodPost);
    rw->setProto(1, 1);
    rw->setContentType("application/json");
    rw->setContentLength(data->str().size());
    if (!rw->sendRequest())
        return false;

    stream::copy(*data, *rw);

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
