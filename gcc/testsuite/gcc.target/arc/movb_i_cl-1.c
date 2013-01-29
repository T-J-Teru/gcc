/* { dg-do compile } */
/* { dg-options "-O2 -mbitops" } */

int
f (int i)
{
  return i & 0x0ffff000;
}
/* { dg-final { scan-assembler "movb\.cl" } } */
