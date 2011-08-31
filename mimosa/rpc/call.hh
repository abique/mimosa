#ifndef MIMOSA_RPC_CALL_HH
# define MIMOSA_RPC_CALL_HH

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

      inline Call(Request  * request  = nullptr,
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

#endif /* !MIMOSA_RPC_CALL_HH */
