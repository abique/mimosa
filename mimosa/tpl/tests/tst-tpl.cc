#include <gtest/gtest.h>

#include "../template.hh"
#include "../value.hh"
#include "../list.hh"
#include "../../stream/string-stream.hh"

namespace mimosa
{
  namespace tpl
  {
    namespace
    {
      TEST(Tpl, ParsingOk)
      {
        const char * templates[] = {
          "hello {{.}}",
          "hello {{.}} xx {{.}} yy",
          " {{*tutu}} {{x}} {{/}} ",
          " {{*tutu}} {{x}} {{/}}",
          " {{*tutu}} {{x}} {{*!}} {{yy}} {{/}} ",
          " {{!toto}} {{u}} {{/}} ",
          nullptr
        };

        for (const char ** it = templates; *it; ++it)
        {
          Template::Ptr tpl = Template::parseString(*it);
          EXPECT_TRUE(tpl);
          if (!tpl)
            std::cout << "template: " << *it << std::endl;;
        }
      }

      TEST(Tpl, Value)
      {
        Template::Ptr tpl = Template::parseString("hello {{value1}}");
        ASSERT_TRUE(tpl);

        Value<std::string> value("world!", "value1");

        stream::StringStream::Ptr stream = new stream::StringStream();
        tpl->execute(stream, value);

        ASSERT_EQ("hello world!", stream->str());
      }

      TEST(Tpl, ValueDot)
      {
        Template::Ptr tpl = Template::parseString("hello {{.}}");
        ASSERT_TRUE(tpl);

        Value<std::string> value("world!");

        stream::StringStream::Ptr stream = new stream::StringStream();
        tpl->execute(stream, value);

        ASSERT_EQ("hello world!", stream->str());
      }

      TEST(Tpl, 2ValueDot)
      {
        Template::Ptr tpl = Template::parseString("hello {{.}} {{.}}");
        ASSERT_TRUE(tpl);

        Value<int> value(2);

        stream::StringStream::Ptr stream = new stream::StringStream();
        tpl->execute(stream, value);

        ASSERT_EQ("hello 2 2", stream->str());
      }

      TEST(Tpl, List1)
      {
        Template::Ptr tpl = Template::parseString("hello{{*.}} {{.}}{{*!}}no entries{{/}}");
        ASSERT_TRUE(tpl);

        List list;

        list.append(new Value<int>(42));
        list.append(new Value<std::string>("toto"));
        list.append(new Value<float>(3.14));

        stream::StringStream::Ptr stream = new stream::StringStream();
        tpl->execute(stream, list);

        ASSERT_EQ("hello 42 toto 3.14", stream->str());
      }

      TEST(Tpl, List2)
      {
        Template::Ptr tpl = Template::parseString("{{*.}}xx{{*!}}yy{{/}}");
        ASSERT_TRUE(tpl);

        List list;

        stream::StringStream::Ptr stream = new stream::StringStream();
        tpl->execute(stream, list);

        ASSERT_EQ("yy", stream->str());
      }
    }
  }
}
