#include "database.hh"

namespace mimosa
{
  namespace rpc
  {
    namespace samples
    {
      void
      Database::get(Call<pb::Key, pb::Result>::Ptr call)
      {
        SharedMutex::ReadLocker locker(lock_);
        printf("get(%s)\n", call->request().key().c_str());
        auto it = kv_.find(call->request().key());
        if (it == kv_.end())
          call->response().set_status(pb::kNotFound);
        else
        {
          call->response().set_value(it->second);
          call->response().set_status(pb::kOk);
        }
      }

      void
      Database::set(Call<pb::KeyValue, pb::Result>::Ptr call)
      {
        SharedMutex::ReadLocker locker(lock_);
        printf("set(%s, %s)\n", call->request().key().c_str(),
               call->request().value().c_str());
        kv_[call->request().key()] = call->request().value();
        call->response().set_status(pb::kOk);
      }

      void
      Database::del(Call<pb::Key, pb::Result>::Ptr call)
      {
        SharedMutex::Locker locker(lock_);
        printf("del(%s)\n", call->request().key().c_str());
        auto it = kv_.find(call->request().key());
        if (it == kv_.end())
          call->response().set_status(pb::kNotFound);
        else
        {
          kv_.erase(it);
          call->response().set_status(pb::kOk);
        }
      }
    }
  }
}
