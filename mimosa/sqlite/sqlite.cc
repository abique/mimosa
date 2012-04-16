#include "../log/log.hh"
#include "../log/origin.hh"
#include "sqlite.hh"

namespace mimosa
{
  namespace sqlite
  {
    log::Origin * log_sqlite = new log::Origin("sqlite", log::kWarning);

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
        log_sqlite->error("failed to open database: %s: %s",
                          filename, sqlite3_errmsg(db_));
        sqlite3_close(db_);
        db_ = nullptr;
      }
      return err;
    }

    Stmt&&
    Db::prepare(const char *str, size_t len)
    {
      Stmt stmt;
      stmt.prepare(db_, str, len);
      return std::move(stmt);
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

    Stmt &
    Stmt::prepare(sqlite3 *    db,
                  const char * sql,
                  int          sql_size)
    {
      assert(!stmt_);
      int err = sqlite3_prepare_v2(db, sql, sql_size, &stmt_, nullptr);
      if (err != SQLITE_OK)
      {
        log_sqlite->error("failed to prepare statement (error: %d): %s",
                          err, sql);
        sqlite3_finalize(stmt_);
        throw nullptr;
      }
      return *this;
    }

    void
    Stmt::reset()
    {
      assert(stmt_);
      sqlite3_reset(stmt_);
    }

    Stmt&
    Stmt::bind(int pos, int value)
    {
      assert(stmt_);
      int err = sqlite3_bind_int(stmt_, pos, value);
      if (err != SQLITE_OK) {
        log_sqlite->error("failed to bind (%v: %v, error: %v) for %v",
                          pos, value, err, sqlite3_sql(stmt_));
        throw nullptr;
      }
      return *this;
    }

    Stmt&
    Stmt::bind(int pos, int64_t value)
    {
      assert(stmt_);
      int err = sqlite3_bind_int64(stmt_, pos, value);
      if (err != SQLITE_OK) {
        log_sqlite->error("failed to bind (%v: %v, error: %v) for %v",
                          pos, value, err, sqlite3_sql(stmt_));
        throw nullptr;
      }
      return *this;
    }

    Stmt&
    Stmt::bind(int pos, double value)
    {
      assert(stmt_);
      int err = sqlite3_bind_double(stmt_, pos, value);
      if (err != SQLITE_OK) {
        log_sqlite->error("failed to bind (%v: %v, error: %v) for %v",
                          pos, value, err, sqlite3_sql(stmt_));
        throw nullptr;
      }
      return *this;
    }

    Stmt&
    Stmt::bind(int pos, const char * value, int value_size)
    {
      assert(stmt_);
      int err = sqlite3_bind_text(stmt_, pos, value, value_size, nullptr);
      if (err != SQLITE_OK) {
        log_sqlite->error("failed to bind (%d: %s, error: %d) for %s",
                          pos, value, err, sqlite3_sql(stmt_));
        throw nullptr;
      }
      return *this;
    }

    Stmt&
    Stmt::bind(int pos, const void * value, int nbytes)
    {
      assert(stmt_);
      int err;

      if (!value)
        err = sqlite3_bind_null(stmt_, pos);
      else
        err = sqlite3_bind_blob(stmt_, pos, value, nbytes, nullptr);

      if (err != SQLITE_OK) {
        log_sqlite->error("failed to bind blob (pos %d: error: %d) for %s",
                          pos, err, sqlite3_sql(stmt_));
        throw nullptr;
      }
      return *this;
    }
  }
}
