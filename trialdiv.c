#include<stdio.h>
#include<stdlib.h>
#include<openssl/bn.h>
#include<stdint.h>
#include<string.h>
#include<math.h>

#include "utilities.h"

void performTrialDivision(unsigned char* number, char* primesfileName)
{
	//printf("trialdiv: number=%s primesfile=%s\n", number, primesfileName);

	unsigned int* numbers;
	int sizeOfNumbers;

	numbers = readPrimesFile(primesfileName, &sizeOfNumbers);

	if(numbers == NULL) //primesfile is not proper
		return;

	BIGNUM* dividend = NULL;

	if (BN_dec2bn(&dividend, (char*)number) == NULL) //convert number to BIGNUM representation
	{
		fprintf(stderr, "Error converting to BIGNUM.\n");
		return;
	}

	trialDivision(number, numbers, sizeOfNumbers, 1, 0);

	free(numbers);
	BN_free(dividend);
}
