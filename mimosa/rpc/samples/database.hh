#ifndef MIMOSA_RPC_SAMPLE_DATABASE_HH
# define MIMOSA_RPC_SAMPLE_DATABASE_HH

# include <mimosa/shared-mutex.hh>

# include "kv-db.pb.h"

namespace mimosa
{
  namespace rpc
  {
    namespace samples
    {
      class Database : public pb::Database::Server
      {
      public:
        // You have to implement the following rpc methods
        virtual bool get(pb::Key & request, pb::Result & response);

        virtual bool set(pb::KeyValue & request, pb::Result & response);

        virtual bool del(pb::Key & request, pb::Result & response);
        // End of rpc methods

      private:
        SharedMutex                        lock_;
        std::map<std::string, std::string> kv_;
      };
    }
  }
}

#endif /* !MIMOSA_RPC_SAMPLE_DATABASE_HH */
