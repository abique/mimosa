#include <sstream>

#include "mail.hh"

namespace mimosa
{
  namespace smtp
  {
    std::string
    Mail::data() const
    {
      std::ostringstream os;
      os << "From: " << from << "\r\n";

      bool first = true;
      for (auto & it : to) {
        if (first)
          os << "To: ";
        else
          os << ", ";
        os << it;
        first = false;
      }
      if (!first)
        os << "\r\n";

      first = true;
      for (auto & it : cc) {
        if (first)
          os << "Cc: ";
        else
          os << ", ";
        os << it;
        first = false;
      }
      if (!first)
        os << "\r\n";

      os << "Date: " << date << "\r\n";
      os << "Subject: " << subject << "\r\n";
      os << "\r\n";
      os << content;

      return os.str();
    }
  }
}
