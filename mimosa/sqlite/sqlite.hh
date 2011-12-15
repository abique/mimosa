#ifndef MIMOSA_SQLITE_SQLITE_HH
# define MIMOSA_SQLITE_SQLITE_HH

# include <sqlite3.h>

# include "../non-copyable.hh"

namespace mimosa
{
  namespace sqlite
  {
    struct Db : private NonCopyable
    {
      Db(sqlite3 * db = nullptr);
      ~Db();

      int open(const char * filename,
               int          flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_FULLMUTEX,
               const char * vfs = nullptr);

      inline operator sqlite3 * () const
      {
        return db_;
      }

      sqlite3 * db_;
    };

    class Stmt : private NonCopyable
    {
    public:
      Stmt(sqlite3_stmt * stmt = nullptr);
      ~Stmt();

      int prepare(sqlite3 *    db,
                  const char * sql,
                  int          sql_size = -1);
      int reset();
      int bind(int pos, int value);
      int bind(int pos, const char * value, int value_size = -1);

      inline operator sqlite3_stmt * () const
      {
        return stmt_;
      }

    private:
      sqlite3_stmt * stmt_;
    };
  }
}

#endif /* !MIMOSA_SQLITE_SQLITE_HH */
