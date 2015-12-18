/* { dg-do compile } */
/* { dg-options "-O2 -mdecode" } */

int
decode_i_p0 (int a, int b)
{
  return (a & 0xfff0000f) | 16 << b;
}
/* { dg-final { scan-assembler "decode\[ \t\]" } } */
