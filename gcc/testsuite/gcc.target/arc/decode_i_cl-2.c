/* { dg-do compile } */
/* { dg-options "-O2 -mdecode" } */

decode_i_cl (int a, int b)
{
  b = (b >> 7) & 31;
  return 16 << b;
}
/* { dg-final { scan-assembler "decode\.cl" } } */
