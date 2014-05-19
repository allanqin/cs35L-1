#include <stdio.h>

int main()
{
	printf("#!usr/local/cs/bin/bash\n\n");
	printf("wget http://localhost:12450/");
	for (int k = 0; k < 7000; k++)
		printf("A");
	printf("\n");
}
