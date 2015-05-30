#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<netinet/in.h>

#include "utilities.h"

void generatePrimes(int maxval)
{
	//printf("primes: maxval=%d\n", maxval);

	int numOfBytes = ceil((float)maxval/(float)8); //how many bytes required
	unsigned char array[numOfBytes];

	Eratosthenes((unsigned int)maxval, array);

	//write output to stdout according to primesfile format
	maxval = htonl((unsigned int)maxval);
	fwrite(&maxval, sizeof(uint32_t), 1, stdout);
	unsigned int i=2;
	for(i=2; i<=maxval; i++)
		if(check(array, i))
		{
			i = htonl(i);
			fwrite(&i, sizeof(uint32_t), 1, stdout);
		}
}
