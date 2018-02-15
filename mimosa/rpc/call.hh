#pragma once

# include "basic-call.hh"

namespace mimosa
{
  namespace rpc
  {
    template <typename Request, typename Response>
    class Call : public BasicCall
    {
    public:
      MIMOSA_DEF_PTR(Call<Request, Response>);

      inline explicit Call(Request  * request  = nullptr,
                           Response * response = nullptr)
        : BasicCall(request ? : new Request(),
                    response ? : new Response())
      {
      }

      inline Request & request() const { return *reinterpret_cast<Request *>(request_); }
      inline Response & response() const  { return *reinterpret_cast<Response *>(response_); }

      inline void allocateMessages()
      {
        request_  = request_ ? : new Request;
        response_ = response_ ? : new Response;
      }
    };
  }
}

