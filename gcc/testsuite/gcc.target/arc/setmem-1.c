/* { dg-do compile } */
/* { dg-options "-O2 -munaligned-access" } */

#include <string.h>

void
f (char *d)
{
  static const char a[] = "abcdefghijklmnopqrstuvwxyz";
  memcpy (d, a, 20);
}
/* { dg-final { scan-assembler-not "stb"  } } */
/* { dg-final { scan-assembler-not "memcpy"  } } */
