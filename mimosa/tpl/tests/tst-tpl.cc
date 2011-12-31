#include <gtest/gtest.h>

#include "../template.hh"
#include "../value.hh"
#include "../list.hh"
#include "../dict.hh"
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

        Value<std::string> value("value1", "world!");

        stream::StringStream::Ptr stream = new stream::StringStream();
        tpl->execute(stream, value);

        ASSERT_EQ("hello world!", stream->str());
      }

      TEST(Tpl, Value1)
      {
        Template::Ptr tpl = Template::parseString("hello {{.}}");
        ASSERT_TRUE(tpl);

        Value<std::string> value("world!");

        stream::StringStream::Ptr stream = new stream::StringStream();
        tpl->execute(stream, value);

        ASSERT_EQ("hello world!", stream->str());
      }

      TEST(Tpl, Value2)
      {
        Template::Ptr tpl = Template::parseString("hello {{.}} {{.}}");
        ASSERT_TRUE(tpl);

        Value<int> value(2);

        stream::StringStream::Ptr stream = new stream::StringStream();
        tpl->execute(stream, value);

        ASSERT_EQ("hello 2 2", stream->str());
      }

      TEST(Tpl, Value3)
      {
        Template::Ptr tpl = Template::parseString("{{*.}}{{.}}{{/}}");
        ASSERT_TRUE(tpl);

        Value<int> value(2);

        stream::StringStream::Ptr stream = new stream::StringStream();
        tpl->execute(stream, value);

        ASSERT_EQ("2", stream->str());
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

      TEST(Tpl, List3)
      {
        Template::Ptr tpl = Template::parseString("{{!.}}yy{{/}}");
        ASSERT_TRUE(tpl);

        List list;

        stream::StringStream::Ptr stream = new stream::StringStream();
        tpl->execute(stream, list);

        ASSERT_EQ("yy", stream->str());
      }

      TEST(Tpl, List4)
      {
        Template::Ptr tpl = Template::parseString("{{*.}}[{{*.}}{{.}}, {{*!}}empty, {{/}}], {{/}}");
        ASSERT_TRUE(tpl);

        List list;
        list.append(new Value<int>(42));
        auto list2 = new List;
        list2->append(new Value<std::string>("tutu"));
        list2->append(new Value<std::string>("toto"));
        list2->append(new Value<std::string>("tata"));
        list.append(list2);

        stream::StringStream::Ptr stream = new stream::StringStream();
        tpl->execute(stream, list);

        ASSERT_EQ("[42, ], [tutu, toto, tata, ], ", stream->str());
      }

      TEST(Tpl, Dict1)
      {
        Template::Ptr tpl = Template::parseString("{{a}} {{b}} {{c}}");
        ASSERT_TRUE(tpl);

        Dict dict;
        dict.append("a", 42);
        dict.append("b", "tutu");
        dict.append("c", "hehe");

        stream::StringStream::Ptr stream = new stream::StringStream();
        tpl->execute(stream, dict);

        ASSERT_EQ("42 tutu hehe", stream->str());
      }

      TEST(Tpl, Dict2)
      {
        Template::Ptr tpl = Template::parseString("{{*.}}{{.}}{{/}}");
        ASSERT_TRUE(tpl);

        Dict dict;
        dict.append("a", 42);

        stream::StringStream::Ptr stream = new stream::StringStream();
        tpl->execute(stream, dict);

        ASSERT_EQ("42", stream->str());
      }

      TEST(Tpl, Dict3)
      {
        Template::Ptr tpl = Template::parseString("{{a.b}}");
        ASSERT_TRUE(tpl);

        Dict dict;
        Dict * dict2 = new Dict("a");
        dict2->append("b", 42);
        dict.append(dict2);

        ASSERT_NE(dict.lookup("a"), nullptr);
        ASSERT_NE(dict2->lookup("b"), nullptr);
        ASSERT_NE(dict.lookup("a")->lookup("b"), nullptr);

        stream::StringStream::Ptr stream = new stream::StringStream();
        tpl->execute(stream, dict);

        ASSERT_EQ("42", stream->str());
      }

      TEST(Tpl, FilterHtml)
      {
        Template::Ptr tpl = Template::parseString("{{.|h}}");
        ASSERT_TRUE(tpl);

        Value<std::string> value("ho'ho");

        stream::StringStream::Ptr stream = new stream::StringStream();
        tpl->execute(stream, value);

        ASSERT_EQ("ho&apos;ho", stream->str());
      }
    }
  }
}
