#include<stdio.h>
#include<stdlib.h>
#include "absolute.h"

int main()
{
	double x, y;
	printf("\nDigite o valor float a ser considerado...\n");
	scanf("%lf", &x);
	
	y = absoluto(x);
	
	printf("O valor absoluto do seu input é: %lf\n", y);
	
	return 0;
}
