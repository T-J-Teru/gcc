/* { dg-do compile } */
/* { dg-options "-mcpu=arcem -Os -Wall -Werror" } */

/* { dg-final { scan-assembler-not "add_s\[ \t\]+r\[0-9\]+,r\[0-9\]+,pcl" } } */

/* Unfortunately the following pragma is required in order to allow
   compiling with -Wall -Werror, but still skip one of the possible switch
   cases in this test.  */
#pragma GCC diagnostic ignored "-Wswitch"

typedef struct {
  struct {
    struct {
      enum {
        REG_SAVED_OFFSET,
        REG_SAVED_REG,
        REG_SAVED_EXP,
        REG_SAVED_VAL_OFFSET,
        REG_SAVED_VAL_EXP
      } how;
    } reg[1];
  } regs;
} _Unwind_FrameState;

_Unwind_FrameState a;

extern void fn2 (void);

void fn1() {
  switch (a.regs.reg[0].how) {
  case REG_SAVED_OFFSET:
    /* Specifically don't handle REG_SAVED_REG in this switch statement,
       this causes GCC to generate the specific switch-jump-vector table
       that triggered a bug on ARCv2 targets.  */
    /* case REG_SAVED_REG: */
  case REG_SAVED_EXP:
  case REG_SAVED_VAL_OFFSET:
    fn2();
  case REG_SAVED_VAL_EXP:
    fn2();
  }
}
