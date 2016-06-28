/* Checks for a bug where static data with a section attribute within a
   function would stop the function being partitioned into hot and cold
   blocks.  */
/* { dg-require-effective-target freorder } */
/* { dg-options "-O2 -fno-profile-reorder-functions -freorder-blocks-and-partition -save-temps" } */

#define SIZE 10000

const char *sarr[SIZE];
const char *buf_hot;
const char *buf_cold;

void foo (int path);

int
main (int argc, char *argv[])
{
  int i;
  buf_hot =  "hello";
  buf_cold = "world";
  for (i = 0; i < 1000000; i++)
    foo (argc);
  return 0;
}

__attribute__((noinline))
void
foo (int path)
{
  static int i __attribute__((section(".data")));
  if (path)
    {
      for (i = 0; i < SIZE; i++)
	sarr[i] = buf_hot;
    }
  else
    {
      for (i = 0; i < SIZE; i++)
	sarr[i] = buf_cold;
    }
}

/* { dg-final-use { scan-assembler "\.section\[\t \]*\.text\.unlikely\[\\n\\r\]+\[\t \]*\.size\[\t \]*foo\.cold\.0" { target *-*-linux* *-*-gnu* } } } */
