/* { dg-do compile { target arc*-mellanox-* } } */
/* { dg-options "-O2 -munaligned-access" } */

extern void *memcpy (void *, const void *, __SIZE_TYPE__);

void
f (char *d)
{
  const char a[26] = "abcdefghijklmnopqrstuvwxyz";
  memcpy (d, a, 20);
}
/* { dg-final { scan-assembler-not "stb"  } } */
/* { dg-final { scan-assembler-not "memcpy"  } } */
