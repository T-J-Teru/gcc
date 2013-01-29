/* { dg-do compile } */
/* { dg-options "-O2 -mdecode" } */

decode_i_pn (int a, int b)
{
  b = (b >> 5) & 31;
  return (a & 0xfff0000f) | 1 << (b + 4);
}
/* { dg-final { scan-assembler "decode\[ \t\]" } } */
