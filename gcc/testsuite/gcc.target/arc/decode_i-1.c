/* { dg-do compile } */
/* { dg-options "-O2 -mdecode" } */

decode_i_p0 (int a, int b)
{
  return (a & 0xfff0000f) | 1 << (b + 4);
}
/* { dg-final { scan-assembler "decode\[ \t\]" } } */
