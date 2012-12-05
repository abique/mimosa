#include "database.hh"

namespace mimosa
{
  namespace rpc
  {
    namespace samples
    {
      bool
      Database::get(pb::Key & request, pb::Result & response)
      {
        SharedMutex::ReadLocker locker(lock_);
        printf("get(%s)\n", request.key().c_str());
        auto it = kv_.find(request.key());
        if (it == kv_.end())
          response.set_status(pb::kNotFound);
        else
        {
          response.set_value(it->second);
          response.set_status(pb::kOk);
        }
        return true;
      }

      bool
      Database::set(pb::KeyValue & request, pb::Result & response)
      {
        SharedMutex::ReadLocker locker(lock_);
        printf("set(%s, %s)\n", request.key().c_str(),
               request.value().c_str());
        kv_[request.key()] = request.value();
        response.set_status(pb::kOk);
        return true;
      }

      bool
      Database::del(pb::Key & request, pb::Result & response)
      {
        SharedMutex::Locker locker(lock_);
        printf("del(%s)\n", request.key().c_str());
        auto it = kv_.find(request.key());
        if (it == kv_.end())
          response.set_status(pb::kNotFound);
        else
        {
          kv_.erase(it);
          response.set_status(pb::kOk);
        }
        return true;
      }
    }
  }
}
