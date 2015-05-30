#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "utilities.h"

void performMaurer(int numbits, char* primesfileName, char* rndfileName)
{
	//printf("maurer: numbits=%d primesfile=%s rndfile=%s\n", numbits, primesfileName, rndfileName);

	//check for errors
	FILE* primesfile = NULL;
	FILE* rndfile = NULL;

	if(numbits <= 0)
	{
		fprintf(stderr, "numbits should be > 0\n");
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

	Maurer(numbits, primesfileName, rndfile, 0);

	fclose(primesfile);
	fclose(rndfile);
}
