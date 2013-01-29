/* { dg-do compile } */
/* { dg-options "-O2 -mbitops" } */

struct { unsigned a: 5, b: 8, c: 8, d: 11; } foo;
struct { unsigned a: 3, b: 8, c: 21; } bar;

void
f (void)
{
  foo.b = foo.c;
  foo.c = bar.b;
}
/* { dg-final { scan-assembler "mrgb\[ \t\]+r\[0-5\]+, *r\[0-5\]+, *r\[0-5\]+, *5, *13, *8, *13, *3, *8" { target arc-*-* } } } */
/* { dg-final { scan-assembler "mrgb\[ \t\]+r\[0-5\]+, *r\[0-5\]+, *r\[0-5\]+, *19, *11, *8, *11, *21, *8" { target arceb-*-* } } } */
