#include <stdio.h>

int main(int arg,char *argv[]) {

int a, b, c;

a = atoi(argv[1]);
b = atoi(argv[2]);

c = a - b;
   
printf("%d - %d = %d\n", a, b,c);
	
return 0;

}
