/* { dg-do compile } */
/* { dg-options "-O2 -mdecode" } */

int
decode_i_cl_plus (int a, int b)
{
  b = (b >> 7) & 31;
  return 1 << (b + 4);
}
/* { dg-final { scan-assembler "decode\.cl" } } */
