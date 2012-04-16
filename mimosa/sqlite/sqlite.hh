#ifndef MIMOSA_SQLITE_SQLITE_HH
# define MIMOSA_SQLITE_SQLITE_HH

# include <string>
# include <cstdint>

# include <sqlite3.h>

# include "../non-copyable.hh"
# include "../string/string-ref.hh"

namespace mimosa
{
  namespace sqlite
  {
    /**
     * @code
     db->prepare("select ... where a = ? and b = ?")
     .bindv(32, "tchac").step().fetch(&val1, &val2);
     @endcode
    */

    class Stmt;

    class Db : private NonCopyable
    {
    public:
      Db(sqlite3 * db = nullptr);
      ~Db();

      int open(const char * filename,
               int          flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_FULLMUTEX,
               const char * vfs = nullptr);

      inline operator sqlite3 * () const
      {
        return db_;
      }

      Stmt&& prepare(const char *str, size_t len = 0);

      sqlite3 * db_;
    };

    class Stmt : private NonCopyable
    {
    public:
      Stmt(sqlite3_stmt * stmt = nullptr);
      ~Stmt();

      void reset();

      Stmt& prepare(sqlite3 *    db,
                    const char * sql,
                    int          sql_size = -1);


      Stmt& bind(int pos, int value);
      Stmt& bind(int pos, int64_t value);
      Stmt& bind(int pos, double value);
      // string
      Stmt& bind(int pos, const char * value, int nbytes = -1);
      // blob or null if value == nullptr
      Stmt& bind(int pos, const void * value, int nbytes);

      // helpers
      inline Stmt& bind(int pos, const std::string & str)
      { return bind(pos, str.c_str(), str.size()); }
      inline Stmt& bind(int pos, const string::StringRef & str)
      { return bind(pos, str.data(), str.size()); }

      // variadic template
      template <typename ...Args>
      inline Stmt& bindv(Args ... args);

      inline int step() { return sqlite3_step(stmt_); }

      inline operator sqlite3_stmt * () const
      {
        return stmt_;
      }

    private:
      template <typename ...Args>
      inline Stmt& bindChain(int pos, int value, Args ... args);
      template <typename ...Args>
      inline Stmt& bindChain(int pos, int64_t value, Args ... args);
      template <typename ...Args>
      inline Stmt& bindChain(int pos, double value, Args ... args);
      template <typename ...Args>
      inline Stmt& bindChain(int pos, const char * value, int nbytes, Args ... args);
      template <typename ...Args>
      inline Stmt& bindChain(int pos, const void * value, int nbytes, Args ... args);
      template <typename ...Args>
      inline Stmt& bindChain(int pos, const std::string & value, Args ... args);
      template <typename ...Args>
      inline Stmt& bindChain(int pos, const string::StringRef & value, Args ... args);

      sqlite3_stmt * stmt_;
    };
  }
}

# include "sqlite.hxx"

#endif /* !MIMOSA_SQLITE_SQLITE_HH */
