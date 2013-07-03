/* Check that we use the clri insn by checking assembler output. */
/* { dg-do compile } */
/* { dg-skip-if "Not available for ARCv1" { *-*-* } { "-mA7" "-mA5" "-mA6" "-mARC600" "-mARC700" "-mcpu=A5" "-mcpu=A600" "-mcpu=A601" "-mcpu=A700" } { "" } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler "\tkflag.ne.*r0" } } */
/* { dg-final { scan-assembler "\tkflag.*16" } } */
void foo (int x)
{
  if (x)
    __builtin_arc_kflag(x);
}

void bar (void)
{
  __builtin_arc_kflag(0x10);
}
