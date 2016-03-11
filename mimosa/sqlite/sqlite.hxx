namespace mimosa
{
  namespace sqlite
  {
    inline Stmt&
    Stmt::exec()
    {
      if (step() != SQLITE_DONE)
        throw nullptr;
      return *this;
    }

    template <typename ...Args>
    inline Stmt&
    Stmt::bind(Args&& ... args)
    {
      assert(stmt_);
      return bindChain(1, args...);
    }

    template <typename ...Args>
    inline Stmt&
    Stmt::bindChain(int pos, int value, Args&& ... args)
    {
      sqlite3_bind_int(stmt_, pos, value);
      return bindChain(pos + 1, args...);
    }

    template <typename ...Args>
    inline Stmt&
    Stmt::bindChain(int pos, unsigned int value, Args&& ... args)
    {
      sqlite3_bind_int(stmt_, pos, value);
      return bindChain(pos + 1, args...);
    }

    template <typename ...Args>
    inline Stmt&
    Stmt::bindChain(int pos, int64_t value, Args&& ... args)
    {
      sqlite3_bind_int64(stmt_, pos, value);
      return bindChain(pos + 1, args...);
    }

    template <typename ...Args>
    inline Stmt&
    Stmt::bindChain(int pos, uint64_t value, Args&& ... args)
    {
      sqlite3_bind_int64(stmt_, pos, value);
      return bindChain(pos + 1, args...);
    }

    template <typename ...Args>
    inline Stmt&
    Stmt::bindChain(int pos, double value, Args&& ... args)
    {
      sqlite3_bind_double(stmt_, pos, value);
      return bindChain(pos + 1, args...);
    }

    template <typename ...Args>
    inline Stmt&
    Stmt::bindChain(int pos, const char * value, int nbytes, Args&& ... args)
    {
      sqlite3_bind_text(stmt_, pos, value, nbytes, nullptr);
      return bindChain(pos + 1, args...);
    }

    template <typename ...Args>
    inline Stmt&
    Stmt::bindChain(int pos, const void * value, int nbytes, Args&& ... args)
    {
      if (!value)
        sqlite3_bind_null(stmt_, pos);
      else
        sqlite3_bind_blob(stmt_, pos, value, nbytes, nullptr);
      return bindChain(pos + 1, args...);
    }

    template <typename ...Args>
    inline Stmt&
    Stmt::bindChain(int pos, const std::string & value, Args&& ... args)
    {
      sqlite3_bind_text(stmt_, pos, value.data(), value.size(), nullptr);
      return bindChain(pos + 1, args...);
    }

    template <typename ...Args>
    inline Stmt&
    Stmt::bindChain(int pos, const StringRef & value, Args&& ... args)
    {
      sqlite3_bind_text(stmt_, pos, value.data(), value.size(), nullptr);
      return bindChain(pos + 1, args...);
    }

    template <typename ... Args>
    inline bool
    Stmt::fetch(Args&& ... args)
    {
      assert(stmt_);
      int ret = step();
      if (ret != SQLITE_ROW)
        return false;

      fetchChain(0, args...);
      return true;
    }

    template <typename ...Args>
    inline void
    Stmt::fetchChain(int pos, bool * value, Args&& ... args)
    {
      *value = sqlite3_column_int(stmt_, pos);
      fetchChain(pos + 1, args...);
    }

    template <typename ...Args>
    inline void
    Stmt::fetchChain(int pos, int * value, Args&& ... args)
    {
      *value = sqlite3_column_int(stmt_, pos);
      fetchChain(pos + 1, args...);
    }

    template <typename ...Args>
    inline void
    Stmt::fetchChain(int pos, int64_t * value, Args&& ... args)
    {
      *value = sqlite3_column_int64(stmt_, pos);
      fetchChain(pos + 1, args...);
    }

    template <typename ...Args>
    inline void
    Stmt::fetchChain(int pos, uint64_t * value, Args&& ... args)
    {
      *value = sqlite3_column_int64(stmt_, pos);
      fetchChain(pos + 1, args...);
    }

    template <typename ...Args>
    inline void
    Stmt::fetchChain(int pos, double * value, Args&& ... args)
    {
      *value = sqlite3_column_double(stmt_, pos);
      fetchChain(pos + 1, args...);
    }

    template <typename ...Args>
    inline void
    Stmt::fetchChain(int pos, const char ** value, int * nbytes, Args&& ... args)
    {
      *value = reinterpret_cast<const char *> (sqlite3_column_text(stmt_, pos));
      *nbytes = sqlite3_column_bytes(stmt_, pos);
      fetchChain(pos + 1, args...);
    }

    template <typename ...Args>
    inline void
    Stmt::fetchChain(int pos, const void ** value, int * nbytes, Args&& ... args)
    {
      *value = sqlite3_column_blob(stmt_, pos);
      *nbytes = sqlite3_column_bytes(stmt_, pos);
      fetchChain(pos + 1, args...);
    }

    template <typename ...Args>
    inline void
    Stmt::fetchChain(int pos, std::string * value, Args&& ... args)
    {
      value->assign((const char*)sqlite3_column_text(stmt_, pos),
                    sqlite3_column_bytes(stmt_, pos));
      fetchChain(pos + 1, args...);
    }

    template <typename ...Args>
    inline void
    Stmt::fetchChain(int pos, StringRef * value, Args&& ... args)
    {
      *value = StringRef((const char*)sqlite3_column_text(stmt_, pos),
                                 sqlite3_column_bytes(stmt_, pos));
      fetchChain(pos + 1, args...);
    }
  }
}
