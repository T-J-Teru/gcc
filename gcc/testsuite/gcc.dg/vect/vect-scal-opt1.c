/* { dg-do compile } */
/* { dg-require-effective-target vect_shift } */
/* { dg-require-effective-target vect_int } */
/* { dg-options "-O -fdump-tree-veclower" } */

#define vidx(type, vec, idx) (*((type *) &(vec) + idx))
#define vector(elcount, type)  \
__attribute__((vector_size((elcount)*sizeof(type)))) type

short k;

int main (int argc, char *argv[]) {
   vector(8, short) v0 = {argc,1,2,3,4,5,6,7};
   vector(8, short) r1;

   r1 = v0 >> (vector(8, short)){2,2,2,2,2,2,2,2};

   return vidx(short, r1, 0);
}

/* { dg-final { scan-tree-dump-times ">> 2" 1 "veclower" } } */
/* { dg-final { cleanup-tree-dump "veclower" } } */
