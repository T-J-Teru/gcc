/* { dg-do compile } */
/* { dg-options "-O2 -mdecode" } */

decode_i_s1 (int a, int b)
{
  b = (b >> 5) & 31;
  return a  | 1 << (b + 4);
}
/* { dg-final { scan-assembler "decode\.cl\[ \t\]" } } */
