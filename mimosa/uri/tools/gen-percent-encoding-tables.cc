#include <stdio.h>
#include <string.h>

bool isValid(char c)
{
  // always convert '+' to simplify decoding :-)
  return ('A' <= c && c <= 'Z') ||
    ('a' <= c && c <= 'z') ||
    ('0' <= c && c <= '9') ||
    strchr("!*'();:@&=$,/?#[]-_.~", c);
}

int main(void)
{
  for (int i = 0; i < 256; ++i)
  {
    if (i % 64 == 0)
      printf("0b");
    printf("%d", isValid((i & ~63) + (63 - (i & 63))));
    if (i % 64 == 63)
      printf("ULL,\n");
  }
  return 0;
}
