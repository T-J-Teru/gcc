/* { dg-do compile } */
/* { dg-options "-O2 -mdecode" } */

decode_i_p0 (int a, int b)
{
  return (a & 0xfff00000) | 1 << b;
}
/* { dg-final { scan-assembler "decode\[ \t\]" } } */
