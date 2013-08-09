/* { dg-do compile { target arc*-mellanox-* } } */
/* { dg-options "-mcmem" } */

#define CMEM_SECTION_ATTR __attribute__ ((section (".cmem_shared")));

#include "cmem-ld.inc"

/* { dg-final { scan-assembler "xld " } } */
/* { dg-final { scan-assembler "xldw " } } */
/* { dg-final { scan-assembler "xldb " } } */
