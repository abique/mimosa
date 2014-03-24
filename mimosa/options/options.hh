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
///
/// To reassure yourself about \ref initialisation, it is safe to use the options
/// framework with C++ static intializers.

#ifndef MIMOSA_OPTIONS_OPTIONS_HH
# define MIMOSA_OPTIONS_OPTIONS_HH

# include <string>
# include <sstream>
# include <vector>

namespace mimosa
{
  namespace options
  {
    /// This is mother class of options. It provides virtual methods to
    /// help you implement custom behaviour.
    class BasicOption
    {
    public:
      BasicOption(const char * group,
                  const char * name,
                  const char * desc)
        : group_(group), name_(name), desc_(desc)
      {
      }

      virtual ~BasicOption() {}

      /// this method is called with arguments following -${name}
      /// You are expected to update argc and argv for each argument
      /// you consume.
      ///
      /// @param argc the number of strings in argv
      /// @param argv a vector of strings
      virtual bool parse(int & argc, char **& argv) = 0;

      /// you are expected to show the description of your option here.
      virtual void showDesc(std::ostream & os)
      {
        os << "  -" << name_ << " (" << desc_ << ")" << std::endl;
      }

      std::string group_;
      std::string name_;
      std::string desc_;
    };

    /// A switch option represent a bool option with no arguments
    /// following -${name}.
    ///
    /// It's meant to be used for -enable-feature-x for exemple.
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

    /// Option is a simple option with one following argument
    /// typed by T.
    ///
    /// The parsing is done through std::istream >> T&.
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

      T value_;
    };

    template <>
    class Option<std::string> : public BasicOption
    {
    public:
      Option(const char * group,
             const char * name,
             const char * desc,
             std::string  default_value)
        : BasicOption(group, name, desc), value_(default_value)
      {
      }

      virtual bool parse(int & argc, char **& argv)
      {
        if (argc < 1)
          return false;

        value_ = *argv;
        --argc;
        ++argv;
        return true;
      }

      virtual void showDesc(std::ostream & os)
      {
        os << "  -" << name_ << " [=" << value_ << "] (" << desc_ << ")" << std::endl;
      }

      std::string value_;
    };

    /// MultiOption is a simple option with one following argument
    /// typed by T.
    ///
    /// The parsing is done through std::istream >> T&.
    template <typename T>
    class MultiOption : public BasicOption
    {
    public:
      MultiOption(const char * group,
                  const char * name,
                  const char * desc)
        : BasicOption(group, name, desc)
      {
      }

      virtual bool parse(int & argc, char **& argv)
      {
        if (argc < 1)
          return false;

        T value;

        std::istringstream is(*argv);
        is >> value;

        --argc;
        ++argv;

        if (!is.fail()) {
          value_.push_back(value);
          return true;
        }
        return false;
      }

      virtual void showDesc(std::ostream & os)
      {
        os << "  -" << name_ << " [*] (" << desc_ << ")" << std::endl;
      }

      std::vector<T> value_;
    };

    /// This options just displays a message and exits the program
    /// when triggered
    class MessageOption : public BasicOption
    {
    public:
      MessageOption(const char * group,
                    const char * name,
                    const char * desc,
                    const char * message)
        : BasicOption(group, name, desc),
          message_(message)
      {
      }

      virtual bool parse(int & argc, char **& argv) override;

    private:
      const char * message_;
    };

    /// Parses the arguments.
    /// Through it may not modify argv.
    ///
    /// If there is an error, parse will terminate the program with
    /// a call to ::exit.
    void parse(int argc, char ** argv);

    /// This adds a basic option to the options parser.
    /// @return dummy value (helper for static initialisation).
    bool addBasicOption(BasicOption * option);

    /// cleanup memory
    void deinit();

    /// This adds an option to the options parser
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

    /// This adds an option to the options parser
    template <typename T>
    inline
    std::vector<T> *
    addMultiOption(const char * group,
                   const char * name,
                   const char * desc)
    {
      auto opt = new MultiOption<T>(group, name, desc);
      addBasicOption(opt);
      return &opt->value_;
    }

    /// This adds a switch to the options parser
    inline
    bool * addSwitch(const char * group,
                     const char * name,
                     const char * desc)
    {
      auto opt = new SwitchOption(group, name, desc);
      addBasicOption(opt);
      return &opt->value_;
    }

    inline
    const char * addMessage(const char * group,
                            const char * name,
                            const char * desc,
                            const char * message)
    {
      auto opt = new MessageOption(group, name, desc, message);
      addBasicOption(opt);
      return message;
    }
  }
}

#endif /* !MIMOSA_OPTIONS_OPTIONS_HH */
