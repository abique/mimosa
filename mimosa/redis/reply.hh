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
      inline Reply(void *reply) : reply_(reinterpret_cast<redisReply*> (reply)) {}
      inline ~Reply() {
        if (reply_)
          freeReplyObject(reply_);
      }

      inline Reply & operator =(Reply && reply) {
        if (reply_)
          freeReplyObject(reply_);
        reply_ = reply.reply_;
        reply.reply_ = nullptr;
	return *this;
      }

      operator redisReply* () const { return reply_; }
      redisReply * operator ->() const { return reply_; }

    private:
      redisReply *reply_;
    };
  }
}

#endif /* !MIMOSA_REDIS_REPLY_HH */
