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
      std::string data() const;

      std::vector<std::string> to;
      std::vector<std::string> cc;
      std::vector<std::string> bcc;
      std::string from;
      std::string reply_to;
      std::string subject;
      std::string date;
      std::string content;
    };
  }
}

#endif /* !MIMOSA_SMTP_MAIL_HH */
