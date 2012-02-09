#include <gtest/gtest.h>

#include "../parse-query.hh"

namespace mimosa
{
  namespace uri
  {
    namespace
    {
      TEST(ParseQuery, Simple)
      {
        std::string    query("a=b&c=d&e&f&g=h");
        container::kvs kvs;
        parseQuery(query.data(), query.size(), &kvs);

        EXPECT_NE(kvs.find("a"), kvs.end());
        EXPECT_EQ(kvs.find("a")->second, "b");
        EXPECT_NE(kvs.find("c"), kvs.end());
        EXPECT_EQ(kvs.find("c")->second, "d");
        EXPECT_NE(kvs.find("e"), kvs.end());
        EXPECT_EQ(kvs.find("e")->second, "");
        EXPECT_NE(kvs.find("f"), kvs.end());
        EXPECT_EQ(kvs.find("f")->second, "");
        EXPECT_NE(kvs.find("g"), kvs.end());
        EXPECT_EQ(kvs.find("g")->second, "h");
      }

      TEST(ParseQuery, Encoding)
      {
        std::string    query("tutu=hoho%26huhu");
        container::kvs kvs;
        parseQuery(query.data(), query.size(), &kvs);

        EXPECT_NE(kvs.find("tutu"), kvs.end());
        EXPECT_EQ(kvs.find("tutu")->second, "hoho&huhu");
      }

      TEST(ParseQuery, SingleValue)
      {
        std::string    query("tutu=hoho");
        container::kvs kvs;
        parseQuery(query.data(), query.size(), &kvs);

        EXPECT_NE(kvs.find("tutu"), kvs.end());
        EXPECT_EQ(kvs.find("tutu")->second, "hoho");
      }

      TEST(ParseQuery, NoValue)
      {
        std::string    query("tutu");
        container::kvs kvs;
        parseQuery(query.data(), query.size(), &kvs);

        EXPECT_NE(kvs.find("tutu"), kvs.end());
        EXPECT_EQ(kvs.find("tutu")->second, "");
      }

      TEST(ParseQuery, Bad1)
      {
        std::string    query("&tutu");
        container::kvs kvs;
        parseQuery(query.data(), query.size(), &kvs);

        EXPECT_NE(kvs.find("tutu"), kvs.end());
        EXPECT_EQ(kvs.find("tutu")->second, "");
      }

      TEST(ParseQuery, Bad2)
      {
        std::string    query("&tutu&");
        container::kvs kvs;
        parseQuery(query.data(), query.size(), &kvs);

        EXPECT_NE(kvs.find("tutu"), kvs.end());
        EXPECT_EQ(kvs.find("tutu")->second, "");
      }

      TEST(ParseQuery, Bad3)
      {
        std::string    query("=tutu");
        container::kvs kvs;
        parseQuery(query.data(), query.size(), &kvs);

        EXPECT_TRUE(kvs.empty());
      }

      TEST(ParseQuery, Bad4)
      {
        std::string    query("=tutu&a=b");
        container::kvs kvs;
        parseQuery(query.data(), query.size(), &kvs);

        EXPECT_NE(kvs.find("a"), kvs.end());
        EXPECT_EQ(kvs.find("a")->second, "b");
      }
    }
  }
}
