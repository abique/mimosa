#pragma once

# include <hiredis/hiredis.h>

# include <mimosa/non-copyable.hh>

namespace mimosa
{
  namespace redis
  {
    class Reply : public NonCopyable
    {
    public:
      inline Reply(Reply && reply) : reply_(reply.reply_) { reply.reply_ = nullptr; }
      inline explicit Reply(void *reply) : reply_(reinterpret_cast<redisReply*> (reply)) {}
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

