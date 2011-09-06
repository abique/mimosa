#ifndef MIMOSA_RPC_SAMPLE_DATABASE_HH
# define MIMOSA_RPC_SAMPLE_DATABASE_HH

# include <mimosa/sync/rwlock.hh>

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
        virtual void get(Call<pb::Key, pb::Result>::Ptr call);

        virtual void set(Call<pb::KeyValue, pb::Result>::Ptr call);

        virtual void del(Call<pb::Key, pb::Result>::Ptr call);
        // End of rpc methods

      private:
        sync::RWLock                       kv_lock_;
        std::map<std::string, std::string> kv_;
      };
    }
  }
}

#endif /* !MIMOSA_RPC_SAMPLE_DATABASE_HH */
