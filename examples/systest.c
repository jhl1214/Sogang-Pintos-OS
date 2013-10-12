#include <stdio.h>
#include <syscall.h>

int
main ()
{
  int i;
 fibonacci(1);
 fibonacci(6);
 fibonacci(7);

 sum_of_four_integers(1,2,3,4);
 sum_of_four_integers(1,1,1,1);
 sum_of_four_integers(200,200,200,200);

  return 0;
}
