namespace mimosa
{
  namespace format
  {
    inline
    std::ostream &
    format(std::ostream & os,
           const char *   fmt)
    {
      os.write(fmt, ::strlen(fmt));
      return os;
    }

    template <typename T, typename ... Args>
    inline
    std::ostream & format(std::ostream & os,
                          const char *   fmt,
                          const T &      value,
                          Args ...       args)
    {
      const char * p = strchrnul(fmt, '%');

      // write the text before `%'
      os.write(fmt, p - fmt);

      // we didn't find `%'
      if (!*p)
        return os;

      ++p;
      switch (*p)
      {
      case 'p':
      case 'x':
        os << std::noshowbase << std::hex << std::nouppercase << value;
        ++p;
        break;

      case 'P':
      case 'X':
        os << std::noshowbase <<std::hex << std::uppercase << value;
        ++p;
        break;

      case 'o':
        os << std::noshowbase << std::oct << value;
        ++p;
        break;

      case 'd':
      case 'i':
      case 'l':
      case 'u':
        os << std::noshowbase << std::dec << value;
        ++p;
        break;

      case 'v':
        os << value;
        ++p;
        break;

      case 'a':
      case 'A':
      case 'e':
      case 'E':
      case 'f':
      case 'F':
      case 'g':
      case 'G':
        os << value;
        ++p;
        break;

      case '%':
        os << "%";
        ++p;
        break;

      default:
        os << value;
        break;
      }

      return format(os, p, args...);
    }
  }
}
