#include <stdio.h>

int main(int arg,char *argv[]) {

int a=1,b;
    
b = atoi(argv[1]);
    
    
for(int i=2; i<=b; i++)
{
   a *= i;             
}

printf("Factorial of %d! = %d\n", b, a);


return 0;

}

