#ifndef MIMOSA_REDIS_CONTEXT_HH
# define MIMOSA_REDIS_CONTEXT_HH

# include <stdargs>

# include <hiredis/hiredis.h>

# include "../non-copyable.hh"

namespace mimosa
{
  namespace redis
  {
    class Context : public NonCopyable
    {
    public:
      inline Context() : ctx_(nullptr) {}
      inline Context(Context && ctx) : ctx_(ctx.ctx_) { ctx.ctx_ = nullptr; }
      inline ~Context() {
        if (ctx_)
          redisFree(ctx_);
      }

      inline Context & operator =(Context && ctx) {
        if (ctx_)
          redisFree(ctx_);
        ctx_ = ctx.ctx_;
        ctx.ctx_ = nullptr;
      }

      inline bool connect(const char *ip, int port) {
        redisFree(ctx_);
        ctx_ = redisConnect(ip, port);
        return ctx_;
      }

      inline operator (redisContext *)() const { return ctx_; }

      inline Reply command(const char *format, ...)
      {
        va_list args;
        va_start(args, format);
        return Reply(redisvCommand(ctx_, format, args));
      }

    private:
      redisContext *ctx_;
    };
  }
}

#endif /* !MIMOSA_REDIS_CONTEXT_HH */
