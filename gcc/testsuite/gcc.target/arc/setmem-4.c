/* { dg-do compile { target arc*-mellanox-* } } */
/* { dg-options "-O2 -munaligned-access" } */

extern void *memcpy (void *, const void *, __SIZE_TYPE__);

void
f (char *d)
{
  const char a[26] =
    {
      'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
      'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
      'u', 'v', 'w', 'x', 'y', 'z',
    };
  memcpy (d, a, 20);
}
/* { dg-final { scan-assembler-not "stb" } } */
/* { dg-final { scan-assembler-not "memcpy" } } */
