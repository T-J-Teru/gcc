int main ()
{
  static int i;

  return !__builtin_constant_p (&i);
}
