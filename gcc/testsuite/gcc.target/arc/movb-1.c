/* { dg-do compile } */
/* { dg-options "-O2 -mbitops" } */

struct { unsigned a: 5, b: 8, c: 19; } foo;
struct { unsigned a: 3, b: 8, c: 21; } bar;

void
f (void)
{
  bar.b = foo.b;
}
/* { dg-final { scan-assembler "movb\[ \t\]+r\[0-9\]+, *r\[0-9\]+, *r\[0-9\]+, *3, *5, *8" { target arc-*-* } } } */
/* { dg-final { scan-assembler "movb\[ \t\]+r\[0-9\]+, *r\[0-9\]+, *r\[0-9\]+, *21, *19, *8" { target arceb-*-* } } } */
