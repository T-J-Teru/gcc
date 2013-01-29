/* { dg-do compile } */
/* { dg-options "-O2 -mcmem" } */

#include <stdint.h>
#include <stdio.h>

struct example1 {
  uint32_t inner_field;
};

struct example2 {
  uint32_t test_field;
  struct example1 inner_struct;
};

struct example_struct
{
  union {
    struct example2 test_struct;
    uint64_t too_longs;
  };
};

struct example_struct __attribute__((section(".cmem_shared"))) e1;
static struct example_struct __attribute__((section(".cmem_private"))) e2;
static struct example_struct e4;


static inline __always_inline void test_func2(uint32_t var)
{
  printf ("value == %d\n", var);
}

void test_func(void)
{
  static struct example_struct __attribute__((section(".cmem"))) e3;
  uint32_t tmp = e3.test_struct.inner_struct.inner_field;

  e1.test_struct.inner_struct.inner_field = 1;
  printf ("value == %d\n", e1.test_struct.inner_struct.inner_field);

  e2.test_struct.inner_struct.inner_field = 1;
  printf ("value == %d\n", e1.test_struct.inner_struct.inner_field);

  e3.test_struct.inner_struct.inner_field = 1;
  printf ("value == %d\n", e1.test_struct.inner_struct.inner_field);

  test_func2(tmp);

  e4.test_struct.inner_struct.inner_field = 1;
  printf ("value == %d\n", e1.test_struct.inner_struct.inner_field);
}

/* All access to e1, e2 and e3 (load and stores) in –O2 optimization level
   should result in specialized instructions.
   E4 access should use normal load and stores.  */

/* { dg-final { scan-assembler "xld\.di\[ \t\]r\[0-9\]*,\\\[cm:@e1" } } */
/* { dg-final { scan-assembler "xld\.di\[ \t\]r\[0-9\]*,\\\[cm:@e3" } } */
/* { dg-final { scan-assembler-not "xld\.di\[ \t\]r\[0-9\]*,\\\[cm:@e4" } } */
/* { dg-final { scan-assembler "xst\.di\[ \t\]r\[0-9\]*,\\\[cm:@e1" } } */
/* { dg-final { scan-assembler "xst\.di\[ \t\]r\[0-9\]*,\\\[cm:@e2" } } */
/* { dg-final { scan-assembler "xst\.di\[ \t\]r\[0-9\]*,\\\[cm:@e3" } } */
/* { dg-final { scan-assembler-not "xst\.di\[ \t\]r\[0-9\]*,\\\[cm:@e4" } } */
