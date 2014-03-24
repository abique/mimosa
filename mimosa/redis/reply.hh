#ifndef MIMOSA_REDIS_REPLY_HH
# define MIMOSA_REDIS_REPLY_HH

# include <hiredis/hiredis.h>

namespace mimosa
{
  namespace redis
  {
    class Reply : public NonCopyable
    {
    public:
      inline Reply(Reply && reply) : reply_(reply.reply_) { reply.reply_ = nullptr; }
      inline Reply(void *reply) : reply_(reply) {}

      inline Reply & operator =(Reply && reply) {
        if (reply_)
          freeReplyObject(reply_);
        reply_ = reply.reply_;
        reply.reply_ = nullptr;
      }

      ~Reply() {
        if (reply_)
          freeReplyObject(reply_);
      }

      redisReply * operator ->() const { return reply_; }

    private:
      redisReply *reply_;
    };
  }
}

#endif /* !MIMOSA_REDIS_REPLY_HH */
