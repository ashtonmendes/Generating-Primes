#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "utilities.h"

void runMillerRabin(unsigned char* number, char* maxIter, char* primesfileName)
{
	//printf("millerrabin: number=%s, maxiter=%s, primesfile=%s\n", number, maxIter, primesfileName);

	unsigned int* numbers;
	int sizeOfNumbers;

	int maxitr = strtod(maxIter, NULL);

	if(maxitr <= 0)
	{
		fprintf(stderr, "Invalid value for maxitr.\n");
		return;
	}

	numbers = readPrimesFile(primesfileName, &sizeOfNumbers);

	if(numbers == NULL) //primesfile is not proper
		return;

	millerRabin(number, maxitr, numbers, sizeOfNumbers, 1, 0, 1);

	free(numbers);
}
