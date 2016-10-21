#include <gtest/gtest.h>

#include "../url.hh"

namespace mimosa
{
  namespace uri
  {
    namespace
    {
      struct UrlData {
	std::string	raw_url_;
	Scheme		scheme_;
	StringRef	host_;
	uint16_t	port_;
	StringRef	path_;
	StringRef	query_;
	StringRef	anchor_;
      };

      static const UrlData g_data[] = {
	{
	  "http://tutu",
	  kSchemeHttp,
	  "tutu",
	  80,
	  "/",
	  "",
	  "",
	},
	{
	  "https://tutu",
	  kSchemeHttps,
	  "tutu",
	  443,
	  "/",
	  "",
	  "",
	},
	{
	  "http://tutu/",
	  kSchemeHttp,
	  "tutu",
	  80,
	  "/",
	  "",
	  "",
	},
	{
	  "http://tutu/toto",
	  kSchemeHttp,
	  "tutu",
	  80,
	  "/toto",
	  "",
	  "",
	},
	{
	  "http://tutu/toto/tata",
	  kSchemeHttp,
	  "tutu",
	  80,
	  "/toto/tata",
	  "",
	  "",
	},
	{
	  "http://tutu/toto/tata?hihi",
	  kSchemeHttp,
	  "tutu",
	  80,
	  "/toto/tata",
	  "hihi",
	  "",
	},
	{
	  "http://tutu/toto/tata?hihi&haha=toto",
	  kSchemeHttp,
	  "tutu",
	  80,
	  "/toto/tata",
	  "hihi&haha=toto",
	  "",
	},
	{
	  "http://tutu/toto/tata?hihi&haha=toto#my-anchor",
	  kSchemeHttp,
	  "tutu",
	  80,
	  "/toto/tata",
	  "hihi&haha=toto",
	  "my-anchor",
	},
	{
	  "http://tutu:79/toto/",
	  kSchemeHttp,
	  "tutu",
	  79,
	  "/toto/",
	  "",
	  "",
	},
          {
            "http://192.168.7.115:2510/api/pullTestData",
            kSchemeHttp,
            "192.168.7.115",
            2510,
            "/api/pullTestData",
            "",
            "",
          },
      };

      void test_valid_url(const UrlData & data)
      {
	Url url;
	std::string parse_error;

	std::cout << "Parsing: " << data.raw_url_ << std::endl;
	bool succeed = url.parse(data.raw_url_, &parse_error);
	if (!succeed) {
	  std::cout << "  error: " << parse_error << std::endl;
	  EXPECT_TRUE(false);
	} else {
	  EXPECT_EQ(url.scheme(), data.scheme_);
	  EXPECT_EQ(url.host(), data.host_);
	  EXPECT_EQ(url.port(), data.port_);
	  EXPECT_EQ(url.path(), data.path_);
	  EXPECT_EQ(url.query(), data.query_);
	  EXPECT_EQ(url.anchor(), data.anchor_);
	}
      }

      TEST(Url, All)
      {
	for (auto & data : g_data)
	  test_valid_url(data);
      }
    }
  }
}
