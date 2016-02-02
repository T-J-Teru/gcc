/* { dg-do compile { target arc*-mellanox-* } } */
/* { dg-options "-O2 -munaligned-access" } */

extern void *memcpy (void *, const void *, __SIZE_TYPE__);

void
f (char *d)
{
  static const char a[] = "abcdefghijklmnopqrstuvwxyz";
  memcpy (d, a, 20);
}
/* { dg-final { scan-assembler-not "stb"  } } */
/* { dg-final { scan-assembler-not "memcpy"  } } */
