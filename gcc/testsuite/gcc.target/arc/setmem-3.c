/* { dg-do compile } */
/* { dg-options "-O2 -munaligned-access" } */

#include <string.h>

void
f (char *d)
{
  const char a[26] = "abcdefghijklmnopqrstuvwxyz";
  memcpy (d, a, 20);
}
/* { dg-final { scan-assembler-not "stb"  } } */
/* { dg-final { scan-assembler-not "memcpy"  } } */
