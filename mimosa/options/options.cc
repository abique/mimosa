#include <iostream>
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
      void invalidArgument(const char * option, const char * arg);

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
        if (argv[0][0] != '-')
        {
          invalidOption(*argv);
          return;
        }

        if (!::strcmp(1 + *argv, "help"))
        {
          showHelp();
          return;
        }

        auto it = options_.find(1 + *argv);
        if (it == options_.end())
        {
          optionNotFound(*argv);
          return;
        }

        const char * option = *argv;
        ++argv;
        --argc;
        const char * value = *argv;

        if (!it->second->parse(argc, argv))
        {
          invalidArgument(option, value);
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

    void
    Parser::duplicateOption(const char * option)
    {
      printf("Duplicate option definition: %s. Terminating.\n", option);
      exit(2);
    }

    void
    Parser::invalidOption(const char * option)
    {
      printf("Invalid option: %s.\n", option);
      exit(2);
    }

    void
    Parser::optionNotFound(const char * option)
    {
      printf("Option not found: %s.\n", option);
      exit(2);
    }

    void
    Parser::invalidArgument(const char * option, const char * arg)
    {
      printf("Invalid argument ``%s'' for option ``%s''.\n",
             arg, option);
      exit(2);
    }

    void
    Parser::showHelp()
    {
      for (auto it = options_.begin(); it != options_.end(); ++it)
        it->second->showDesc(std::cout);
      exit(0);
    }
  }
}
