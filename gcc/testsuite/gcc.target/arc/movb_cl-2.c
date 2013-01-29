/* { dg-do compile } */
/* { dg-options "-O2 -mbitops" } */

extern void g (void);
int
f (int i)
{
  if (i & 0x0ffff000)
    g ();
}
/* { dg-final { scan-assembler "movb\.f\.cl" } } */
