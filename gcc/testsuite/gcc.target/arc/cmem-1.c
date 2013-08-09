/* { dg-do compile { target arc*-mellanox-* } } */
/* { dg-options "-mcmem" } */

#define CMEM_SECTION_ATTR __attribute__ ((section (".cmem")));

#include "cmem-st.inc"

/* { dg-final { scan-assembler "xst " } } */
/* { dg-final { scan-assembler "xstw " } } */
/* { dg-final { scan-assembler "xstb " } } */
