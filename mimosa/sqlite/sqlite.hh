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
     .bindv(32, "tchac").exec();
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

      // variadic template
      template <typename ...Args>
      inline Stmt& bind(Args ... args);

      inline int step() { return sqlite3_step(stmt_); }

      // will throw on error
      inline Stmt& exec();

      /**
       * call sqlite3_step() and if ret == SQLITE_ROW, then fetch values.
       *
       * @return true if we got a row, and false otherwise
       */
      template <typename ... Args>
      inline bool fetch(Args ... args);

      inline operator sqlite3_stmt * () const
      {
        return stmt_;
      }

    private:
      inline Stmt& bindChain(int) { return *this; }
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

      inline void fetchChain(int) {}
      template <typename ...Args>
      inline void fetchChain(int pos, int * value, Args ... args);
      template <typename ...Args>
      inline void fetchChain(int pos, int64_t * value, Args ... args);
      template <typename ...Args>
      inline void fetchChain(int pos, uint64_t * value, Args ... args);
      template <typename ...Args>
      inline void fetchChain(int pos, double * value, Args ... args);
      template <typename ...Args>
      inline void fetchChain(int pos, const char ** value, int * nbytes, Args ... args);
      template <typename ...Args>
      inline void fetchChain(int pos, const void ** value, int * nbytes, Args ... args);
      template <typename ...Args>
      inline void fetchChain(int pos, std::string * value, Args ... args);
      template <typename ...Args>
      inline void fetchChain(int pos, string::StringRef * value, Args ... args);

      sqlite3_stmt * stmt_;
    };
  }
}

# include "sqlite.hxx"

#endif /* !MIMOSA_SQLITE_SQLITE_HH */
