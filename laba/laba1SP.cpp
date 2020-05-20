#include <stdio.h>
#include <stdlib.h>
#include <locale.h> 
#include <limits.h>
#include <float.h>
int main(void)
{
	printf("TYPE\t\t\tSIZE\t\tMIN\t\t\tMAX\n");
	printf("char\t\t\t%i\t\t%i\t\t\t%i\n", sizeof(char), CHAR_MIN, CHAR_MAX);
	printf("signed char\t\t%i\t\t%i\t\t\t%i\n", sizeof(signed char), SCHAR_MIN, SCHAR_MAX);
	printf("unsigned char\t\t%i\t\t%i\t\t\t%i\n", sizeof(unsigned char), 0, UCHAR_MAX);
	printf("\n");
	printf("short\t\t\t%i\t\t%i\t\t\t%i\n", sizeof(short), SHRT_MIN, SHRT_MAX);
	printf("unsigned short\t\t%i\t\t%i\t\t\t%i\n", sizeof(unsigned short), 0, USHRT_MAX);
	printf("\n");
	printf("int\t\t\t\%i\t\t%i\t\t%i\n", sizeof(int), INT_MIN, INT_MAX);
	printf("unsigned int\t\t%i\t\t%i\t\t\t%u\n", sizeof(unsigned int), 0, UINT_MAX);
	printf("\n");
	printf("long\t\t\t%i\t\t%li\t\t%li\n", sizeof(long), LONG_MIN, LONG_MAX);
	printf("unsigned long\t\t%i\t\t%i\t\t\t%lu\n", sizeof(unsigned long), 0, ULONG_MAX);
	printf("long long\t\t%i\t\t%lli\t%lli\n", sizeof(long long), LLONG_MIN, LLONG_MAX);
	printf("unsigned long long\t%i\t\t%i\t\t\t%llu\n", sizeof(unsigned long long), 0, ULLONG_MAX);
	printf("\n");
	printf("float\t\t\t%i\t\t%e\t\t%e\n", sizeof(float), FLT_MIN, FLT_MAX);
	printf("double\t\t\t%i\t\t%e\t\t%e\n", sizeof(double), DBL_MIN, DBL_MAX);
	printf("long double\t\t%i\t\t%e\t\t%e\n", sizeof(double), LDBL_MIN, LDBL_MIN);
	system("pause");
	return 0;
}
