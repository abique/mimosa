#ifndef MIMOSA_SMTP_MAIL_HH
# define MIMOSA_SMTP_MAIL_HH

# include <string>
# include <vector>

namespace mimosa
{
  namespace smtp
  {
    struct Mail
    {
      std::vector<std::string> to;
      std::vector<std::string> cc;
      std::vector<std::string> cci;
      std::string from;
      std::string subject;
      std::string date;
    };
  }
}

#endif /* !MIMOSA_SMTP_MAIL_HH */
