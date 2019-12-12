#include <stdio.h>
#include <stdlib.h>
int main()
{
	int *p = (int *)malloc(5*sizeof(int));
	printf("%d\n", sizeof(p));

	return 0;
}