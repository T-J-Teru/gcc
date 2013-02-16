/* { dg-do compile } */
/* { dg-options "-O2 -mbitops" } */

struct { int a: 23, b: 9; } foo;
struct { int a: 23, b: 9; } bar;

void
f (void)
{
  bar.b = foo.b;
}
/* { dg-final { scan-assembler "movb\[ \t\]+r\[0-9\]+, *r\[0-9\]+, *r\[0-9\]+, *23, *(23|7), *9" { target arc-*-* } } } */
/* { dg-final { scan-assembler "movb\[ \t\]+r\[0-9\]+, *r\[0-9\]+, *r\[0-9\]+, *0, *0, *9" { target arceb-*-* } } } */
