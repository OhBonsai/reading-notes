#include <stdio.h>

struct rec {

		int i;
		int j;
		int a[2];
		int *p;
};

long area(struct rec *rp){
		return rp->i * rp->j;
}


int main(){

		struct rec  r = {0, 0, 10, 20, 0xFF00FF};
		printf("%d\n", r.j);
		return 0;
}
