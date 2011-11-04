/// @page OptionsParser Options parser
///
/// Mimosa introduces a very simple Options parser, yet extensible.
/// Options can be declared per modules.
///
/// As a user you only have to know 3 functions:
/// - mimosa::options::parse to parses argc and argv
/// - mimosa::options::addSwitch to add a switch, like -enable-feature-x
/// - mimosa::options::addOption to add a valued option like -port 19042
///
/// Then if you need to do custom option parsing or what ever,
/// just dig in mimosa::options::BasicOption and
/// mimosa::options::addBasicOptions.
///
/// Here comes a sample code:
/// @code
/// #include <mimosa/options/options.hh>
///
/// bool * enable_x = mimosa::options::addSwitch("group", "enable-x", "enables feature x");
/// std::string * prefix = mimosa::options::addOption<std::string>("group", "prefix", "the prefix path", "/usr/local");
/// uint16_t * port = mimosa::options::addOption<uint16_t>("group", "port", "the port to listen on", 19042);
///
/// int main(int argc, char ** argv)
/// {
///   mimosa::options::parse(argc, argv);
///
///   /* here do what you want */
///
///   return 0;
/// }
/// @endcode

#ifndef MIMOSA_OPTIONS_OPTIONS_HH
# define MIMOSA_OPTIONS_OPTIONS_HH

# include <string>
# include <sstream>

namespace mimosa
{
  namespace options
  {
    class BasicOption
    {
    public:
      BasicOption(const char * group,
                  const char * name,
                  const char * desc)
        : group_(group), name_(name), desc_(desc)
      {
      }

      virtual bool parse(int & argc, char **& argv) = 0;
      virtual void showDesc(std::ostream & os)
      {
        os << "  -" << name_ << " (" << desc_ << ")" << std::endl;
      }

      std::string group_;
      std::string name_;
      std::string desc_;
    };

    class SwitchOption : public BasicOption
    {
    public:
      SwitchOption(const char * group,
                   const char * name,
                   const char * desc)
        : BasicOption(group, name, desc), value_(false)
      {
      }

      virtual bool parse(int &, char **&)
      {
        value_ = true;
        return true;
      }

      bool value_;
    };

    template <typename T>
    class Option : public BasicOption
    {
    public:
      Option(const char * group,
             const char * name,
             const char * desc,
             T            default_value)
        : BasicOption(group, name, desc), value_(default_value)
      {
      }

      virtual bool parse(int & argc, char **& argv)
      {
        if (argc < 1)
          return false;

        std::istringstream is(*argv);
        is >> value_;

        --argc;
        ++argv;

        return !is.fail();
      }

      virtual void showDesc(std::ostream & os)
      {
        os << "  -" << name_ << " [=" << value_ << "] (" << desc_ << ")" << std::endl;
      }

      T           value_;
    };

    void parse(int argc, char ** argv);
    void addBasicOption(BasicOption * option);

    template <typename T>
    inline
    T * addOption(const char * group,
                  const char * name,
                  const char * desc,
                  T            default_value = T())
    {
      auto opt = new Option<T>(group, name, desc, default_value);
      addBasicOption(opt);
      return &opt->value_;
    }

    inline
    bool * addSwitch(const char * group,
                     const char * name,
                     const char * desc)
    {
      auto opt = new SwitchOption(group, name, desc);
      addBasicOption(opt);
      return &opt->value_;
    }
  }
}

#endif /* !MIMOSA_OPTIONS_OPTIONS_HH */
