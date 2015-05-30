#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "utilities.h"

void performRndSearch(int numbits, int maxitr, char* primesfileName, char* rndfileName)
{
	//printf("rndsearch: numbits=%d maxitr=%d primesfile=%s rndfile=%s\n", numbits, maxitr, primesfileName, rndfileName);

	//check for errors
	FILE* primesfile = NULL;
	FILE* rndfile = NULL;

	if(numbits <= 0)
	{
		fprintf(stderr, "numbits should be > 0\n");
		return;
	}
	else if(maxitr <= 0)
	{
		fprintf(stderr, "maxitr should be > 0\n");
		return;
	}
	else
	{
		primesfile = fopen(primesfileName, "rb");
		rndfile = fopen(rndfileName, "rb");

		if(primesfile == NULL)
		{
			fprintf(stderr, "Could not open %s.\n", primesfileName);
			return;
		}
		if(rndfile == NULL)
		{
			fprintf(stderr, "Could not open %s.\n", rndfileName);
			return;
		}
	}

	randomSearch(numbits, maxitr, primesfileName, rndfile);

	fclose(primesfile);
	fclose(rndfile);
}
