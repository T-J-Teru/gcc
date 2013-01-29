static int i;

int main ()
{
  int i;

  return __builtin_constant_p (&i);
}
