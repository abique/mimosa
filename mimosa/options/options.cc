#include <cstring>
#include <memory>
#include <map>

#include "options.hh"

namespace mimosa
{
  namespace options
  {
    struct Parser
    {
      ~Parser();

      void parse(int argc, char ** argv);
      void addOption(BasicOption * option);

      void duplicateOption(const char * option);
      void invalidOption(const char * option);
      void optionNotFound(const char * option);
      void invalidValue(const char * option, const char * value);

      void showHelp();

      std::string                             program_name_;
      std::map<std::string,
               std::unique_ptr<BasicOption> > options_;
    };

    Parser parser;

    void
    Parser::parse(int argc, char ** argv)
    {
      if (argc == 0)
        return;

      parser.program_name_ = *argv;
      --argc;
      ++argv;

      while (argc > 0)
      {
        if (argv[0][0] != '-' || argv[0][1] != '-')
        {
          invalidOption(*argv);
          return;
        }

        if (!::strcmp(2 + *argv, "help"))
        {
          showHelp();
          return;
        }

        auto it = options_.find(2 + *argv);
        if (it == options_.end())
        {
          invalidOption(*argv);
          return;
        }

        const char * option = *argv;
        ++argv;
        --argc;
        const char * value = *argv;

        if (!it->second->parse(argc, argv))
        {
          invalidValue(option, value);
          return;
        }
      }
    }

    void
    Parser::addOption(BasicOption * option)
    {
      auto it = options_.find(option->name_);
      if (it != options_.end())
      {
        duplicateOption(option->name_.c_str());
        return;
      }
      options_.insert(
        std::make_pair(option->name_,
                       std::unique_ptr<BasicOption>(option)));
    }
  }
}
