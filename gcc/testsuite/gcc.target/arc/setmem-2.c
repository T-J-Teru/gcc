/* { dg-do compile } */
/* { dg-options "-O2 -munaligned-access" } */

#include <string.h>

void
f (char *d)
{
  static const char a[] =
{
  'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
  'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
  'u', 'v', 'w', 'x', 'y', 'z',
};
  memcpy (d, a, 20);
}
/* { dg-final { scan-assembler-not "stb"  } } */
/* { dg-final { scan-assembler-not "memcpy"  } } */
/* { dg-final { scan-assembler-not "rodata" { xfail *-*-*} } } */
