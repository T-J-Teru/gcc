/* { dg-do compile { target arc*-mellanox-* } } */
/* { dg-options "-O2 -mbitops" } */

struct foo { unsigned a: 3, b: 5, c: 24; };

int
f (struct foo i)
{
  return i.b;
}
/* { dg-final { scan-assembler "movb\.cl" } } */
