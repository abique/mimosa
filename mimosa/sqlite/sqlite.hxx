namespace mimosa
{
  namespace sqlite
  {
    template <typename ...Args>
    inline Stmt&
    Stmt::bindv(Args ... args)
    {
      return bindChain(1, args...);
    }

    template <typename ...Args>
    inline Stmt&
    Stmt::bindChain(int pos, int value, Args ... args)
    {
      bind(pos, value);
      return bindChain(pos + 1, args...);
    }

    template <typename ...Args>
    inline Stmt&
    Stmt::bindChain(int pos, int64_t value, Args ... args)
    {
      bind(pos, value);
      return bindChain(pos + 1, args...);
    }

    template <typename ...Args>
    inline Stmt&
    Stmt::bindChain(int pos, double value, Args ... args)
    {
      bind(pos, value);
      return bindChain(pos + 1, args...);
    }

    template <typename ...Args>
    inline Stmt&
    Stmt::bindChain(int pos, const char * value, int nbytes, Args ... args)
    {
      bind(pos, value, nbytes);
      return bindChain(pos + 1, args...);
    }

    template <typename ...Args>
    inline Stmt&
    Stmt::bindChain(int pos, const void * value, int nbytes, Args ... args)
    {
      bind(pos, value, nbytes);
      return bindChain(pos + 1, args...);
    }

    template <typename ...Args>
    inline Stmt&
    Stmt::bindChain(int pos, const std::string & value, Args ... args)
    {
      bind(pos, value);
      return bindChain(pos + 1, args...);
    }

    template <typename ...Args>
    inline Stmt&
    Stmt::bindChain(int pos, const string::StringRef & value, Args ... args)
    {
      bind(pos, value);
      return bindChain(pos + 1, args...);
    }
  }
}
