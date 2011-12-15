#include "../log/log.hh"
#include "../log/origin.hh"
#include "sqlite.hh"

namespace mimosa
{
  namespace sqlite
  {
    log::Origin * sqlite = new log::Origin("sqlite", log::Warning);

    /////////////
    // Db
    /////////////

    Db::Db(sqlite3 * db)
      : db_(db)
    {
    }

    Db::~Db()
    {
      if (db_)
      {
        sqlite3_close(db_);
        db_ = nullptr;
      }
    }

    int
    Db::open(const char * filename,
             int          flags,
             const char * vfs)
    {
      assert(!db_);
      int err = sqlite3_open_v2(filename, &db_, flags, vfs);

      if (err != SQLITE_OK)
      {
        MIMOSA_LOG(Error, sqlite, "failed to open database: %s: %s",
                   filename, sqlite3_errmsg(db_));
        sqlite3_close(db_);
        db_ = nullptr;
      }
      return err;
    }

    /////////////
    // Stmt
    /////////////

    Stmt::Stmt(sqlite3_stmt * stmt)
      : stmt_(stmt)
    {
    }

    Stmt::~Stmt()
    {
      if (stmt_)
      {
        sqlite3_finalize(stmt_);
        stmt_ = nullptr;
      }
    }

    int
    Stmt::prepare(sqlite3 *    db,
                  const char * sql,
                  int          sql_size)
    {
      assert(!stmt_);
      int err = sqlite3_prepare_v2(db, sql, sql_size, &stmt_, nullptr);
      if (err != SQLITE_OK)
      {
        MIMOSA_LOG(Error, sqlite, "failed to prepare statement (error: %d): %s",
                   err, sql);
        sqlite3_finalize(stmt_);
        stmt_ = nullptr;
      }
      return err;
    }

    int
    Stmt::reset()
    {
      assert(stmt_);
      return sqlite3_reset(stmt_);
    }

    int
    Stmt::bind(int pos, int value)
    {
      assert(stmt_);
      int err = sqlite3_bind_int(stmt_, pos, value);
      if (err != SQLITE_OK)
        MIMOSA_LOG(Error, sqlite, "failed to bind (%d: %d, error: %d) for %s",
                   pos, value, err, sqlite3_sql(stmt_));
      return err;
    }

    int
    Stmt::bind(int pos, const char * value, int value_size)
    {
      assert(stmt_);
      int err = sqlite3_bind_text(stmt_, pos, value, value_size, nullptr);
      if (err != SQLITE_OK)
        MIMOSA_LOG(Error, sqlite, "failed to bind (%d: %s, error: %d) for %s",
                   pos, value, err, sqlite3_sql(stmt_));
      return err;
    }
  }
}
