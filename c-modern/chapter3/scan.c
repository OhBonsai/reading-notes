#include <stdio.h>

int main(void){
  int a,aa,b,bb,r,rr;

  printf("please enter the first number\n");
  scanf("%d/%d", &a, &aa);
  printf("please enter the second number\n");
  scanf("%d/%d", &b, &bb);

  r = a*bb + b*aa;
  rr = aa * bb;

  printf("the result is %d/%d\n", r, rr);

  return 0;
}
