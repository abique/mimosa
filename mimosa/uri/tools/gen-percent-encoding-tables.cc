#include <stdio.h>
#include <string.h>

bool isValid(char c, bool allow_reserved)
{
  // always convert '+' to simplify decoding :-)
  return ('A' <= c && c <= 'Z') ||
    ('a' <= c && c <= 'z') ||
    ('0' <= c && c <= '9') ||
    (allow_reserved && strchr("!*'();:@&=$,/?#[]-_.~", c));
}

void print(bool allow_reserved,
           const char *comment)
{
  printf("/* %s */\n", comment);
  for (int i = 0; i < 256; ++i)
  {
    if (i % 64 == 0)
      printf("0b");
    printf("%d", isValid((i & ~63) + (63 - (i & 63)), allow_reserved));
    if (i % 64 == 63)
      printf("ULL,\n");
  }
}

int main(void)
{
  print(false, "no reserved");
  print(true, "with reserved");
  return 0;
}
