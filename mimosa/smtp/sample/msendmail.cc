#include "../../init.hh"
#include "../..//options/options.hh"

#include "../mail.hh"
#include "../client.hh"

std::string & HOST = *mimosa::options::addOption<std::string>("", "host", "", "");
std::string & TO = *mimosa::options::addOption<std::string>("", "to", "", "");
std::string & FROM = *mimosa::options::addOption<std::string>("", "from", "", "");
std::string & SUBJECT = *mimosa::options::addOption<std::string>("", "subject", "", "");
std::string & DATE = *mimosa::options::addOption<std::string>("", "date", "", "");
std::string & CONTENT = *mimosa::options::addOption<std::string>("", "content", "", "");

int main(int argc, char **argv)
{
  mimosa::init(argc, argv);

  mimosa::smtp::Client client;
  if (!client.connect(HOST, 25))
    return 1;

  if (!client.hello("localhost"))
    return 2;

  mimosa::smtp::Mail mail;
  mail.to.push_back(TO);
  mail.from    = FROM;
  mail.subject = SUBJECT;
  mail.date    = DATE;
  mail.content = CONTENT;

  if (!client.sendMail(mail))
    return 3;

  mimosa::deinit();
  return 0;
}
