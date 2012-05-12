#include <vector>

#include "../string-ref.hh"
#include "normalize-path.hh"

namespace mimosa
{
  namespace uri
  {
    void normalizePath(const char * const  input,
                       uint32_t const      len,
                       std::string * const output)
    {
      std::vector<StringRef> stack;
      const char * const end = input + len;
      const char * in = input;
      bool is_root = (len > 0 && *input == '/');
      bool is_dir = false;

      while (in < end)
      {
        /* skip slashes */
        while (in < end && *in == '/')
            ++in;

        if (in == end)
        {
          is_dir = true;
          break;
        }

        const char * token = in;

        /* find slash */
        while (in < end && *in != '/')
          ++in;

        const char * token_end = in;

        switch (token_end - token)
        {
        case 1:
          if (token[0] == '.')
          {
            is_dir = true;
            continue;
          }
          break;

        case 2:
          if (token[0] == '.' && token[1] == '.')
          {
            if (!stack.empty())
              stack.pop_back();
            is_dir = true;
            continue;
          }
          break;
        }
        stack.push_back(StringRef(token, token_end));
        is_dir = false;
      }

      /* calculating size to reserve the string */
      uint32_t size = 0;
      for (auto it = stack.cbegin(); it != stack.cend(); ++it)
        size += it->size() + 1;
      output->clear();
      output->reserve(size);

      if (stack.empty())
      {
        output->push_back(is_root ? '/' : '.');
        return;
      }

      bool need_slash = is_root;
      for (auto it = stack.cbegin(); it != stack.cend(); ++it)
      {
        if (need_slash)
          output->push_back('/');
        else
          need_slash = true;
        output->append(it->data(), it->size());
      }
      if (is_dir)
        output->push_back('/');
    }
  }
}
