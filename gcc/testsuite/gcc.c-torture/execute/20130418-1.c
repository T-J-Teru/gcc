static int i;

int main ()
{
  return !__builtin_constant_p (&i);
}
