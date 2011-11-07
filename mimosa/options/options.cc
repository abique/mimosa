#include <iostream>
#include <cstring>
#include <string>
#include <memory>
#include <map>
#include <vector>

#include "options.hh"

namespace mimosa
{
  namespace options
  {
    /// This class is an internal class, to parse and manages options.
    struct Parser
    {
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

        if (!::strcmp(1 + *argv, "h") ||
            !::strcmp(1 + *argv, "help"))
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
      std::cout << "Duplicate option definition: " << option
                << ". Terminating." << std::endl;
      exit(2);
    }

    void
    Parser::invalidOption(const char * option)
    {
      std::cout << "Invalid option: " << option << ". Try -help." << std::endl;
      exit(2);
    }

    void
    Parser::optionNotFound(const char * option)
    {
      std::cout << "Option not found: " << option << ". Try -help." << std::endl;
      exit(2);
    }

    void
    Parser::invalidArgument(const char * option, const char * arg)
    {
      std::cout << "Invalid argument ``" << arg << "'' for option ``"
                << option << "''. Try -help." << std::endl;
      exit(2);
    }

    void
    Parser::showHelp()
    {
      std::cout << "Usage: " << program_name_ << " [option]..." << std::endl;

      std::map<std::string, std::vector<BasicOption *> > groups;

      for (auto it = options_.begin(); it != options_.end(); ++it)
        groups[it->second->group_].push_back(it->second.get());

      for (auto group = groups.begin(); group != groups.end(); ++group)
      {
        if (!group->first.empty())
          std::cout << std::endl << "[" << group->first << "]" << std::endl;
        for (auto it = group->second.begin(); it != group->second.end(); ++it)
          (*it)->showDesc(std::cout);
      }
      exit(0);
    }

    void parse(int argc, char ** argv)
    {
      parser.parse(argc, argv);
    }

    bool addBasicOption(BasicOption * option)
    {
      parser.addOption(option);
      return true;
    }
  }
}