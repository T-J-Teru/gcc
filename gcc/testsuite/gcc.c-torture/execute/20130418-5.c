/* { dg-options "-fno-common" } */

int i;

int main ()
{
  return !__builtin_constant_p (&i);
}
