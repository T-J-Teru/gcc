// { dg-do run }
// { dg-options "-save-temps" }

#include "call-with-empty-struct.H"

MAKE_STRUCT_PASSING_TEST(float,2.5)

// { dg-final { scan-assembler "fa0" } }

