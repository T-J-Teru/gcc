/* { dg-do compile { target arc*-mellanox-* } } */
/* { dg-options "-mcmem" } */

#define CMEM_SECTION_ATTR __attribute__ ((section (".cmem_private")));

#include "cmem-st.inc"

/* { dg-final { scan-assembler "xst " } } */
/* { dg-final { scan-assembler "xstw " } } */
/* { dg-final { scan-assembler "xstb " } } */
