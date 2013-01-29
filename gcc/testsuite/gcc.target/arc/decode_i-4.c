/* { dg-do compile } */
/* { dg-options "-O2 -mdecode" } */

int
decode_i_pn (int a, int b)
{
  b = (b >> 5) & 31;
  return (a & 0xfff00000) | 1 << b;
}
/* { dg-final { scan-assembler "decode\[ \t\]" } } */
