#include <stdio.h>

bool isValid(char c, bool allow_plus)
{
  return (('A' <= c && c <= 'Z') ||
          ('a' <= c && c <= 'z') ||
          ('0' <= c && c <= '9') ||
          c == '!' ||
          c == '*' ||
          c == '\'' ||
          c == '(' ||
          c == ')' ||
          c == ';' ||
          c == ':' ||
          c == '@' ||
          c == '&' ||
          c == '=' ||
          (allow_plus && c == '+') ||
          c == '$' ||
          c == ',' ||
          c == '/' ||
          c == '?' ||
          c == '#' ||
          c == '[' ||
          c == ']' ||
          c == '-' ||
          c == '_' ||
          c == '.' ||
          c == '~');
}

int main(void)
{
  for (int plus = 0; plus < 2; ++plus)
    for (int i = 0; i < 256; ++i)
    {
      if (i % 64 == 0)
        printf("0b");
      printf("%d", isValid(i, plus));
      if (i % 64 == 63)
        printf("ULL,\n");
    }
  return 0;
}
