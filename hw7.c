#include <stdio.h>
#include <stdlib.h>
#include<string.h>

#include "primes.h"
#include "trialdiv.h"
#include "millerrabin.h"
#include "rndsearch.h"
#include "maurer.h"

void printUsage()
{
	fprintf(stderr, "Usage:\nhw7 primes -n=maxval\nhw7 trialdiv -n=number -p=primesfile\nhw7 millerrabin -n=number -t=maxitr -p=primesfile\nhw7 rndsearch -k=numbits -t=maxitr -p=primesfile -r=rndfile\nhw7 maurer -k=numbits -p=primesfile -r=rndfile\n");
}

int main(int argc, char* argv[])
{
	//skip argv[0]
	argc--;
	argv++;

	if(argc <= 0) //No arguments provided
	{
		fprintf(stderr, "Please enter command line arguments.\n");
		printUsage();
		return EXIT_FAILURE;
	}

	//primes
	if(strcmp(*argv, "primes") == 0)
	{
		if(argc-1 < 1 || argc-1 > 1)
		{
			fprintf(stderr, "Malformed command.\n");
			printUsage();
			return EXIT_FAILURE;
		}

		char* maxvalString = malloc(100*sizeof(char));
		unsigned int maxval = 0;

		for(argc--; argc>0; argc--)
		{
			argv++;

			if(strlen(*argv) < 4)
			{
				fprintf(stderr, "Malformed command.\n");
				printUsage();
				return EXIT_FAILURE;
			}

			//maxval
			if(argv[0][0]=='-' && argv[0][1]=='n' && argv[0][2]=='=')
			{
				//Get tablefile from argv
				int i=0;
				for(i=3; i<strlen(*argv); i++)
				{
					maxvalString[i-3] = argv[0][i];
				}
				maxvalString[i-3] = '\0';
			}

			else
			{
				fprintf(stderr, "Malformed command.\n");
				printUsage();
				return EXIT_FAILURE;
			}
		}

		maxval = strtod(maxvalString, NULL);
		//check for errors
		if(maxval < 2 || maxval > 16777216) //maxval must be between 2 and 2^24, inclusive
		{
			fprintf(stderr, "maxval must be between 2 and 2^24, inclusive.\n");
			printUsage();
			return EXIT_FAILURE;
		}

		generatePrimes(maxval);

		free(maxvalString);
	}

	//trialdiv
	else if(strcmp(*argv, "trialdiv") == 0)
	{
		//Get hold of arguments
		char* number = malloc(100*sizeof(char));
		char* primesfileName = malloc(100*sizeof(char));

		//No, less or extra parameters for keyexpand
		if(argc-1 < 2 || argc-1 > 2)
		{
			fprintf(stderr, "Malformed command.\n");
			printUsage();
			return EXIT_FAILURE;
		}

		for(argc--; argc>0; argc--)
		{
			argv++;

			if(strlen(*argv) < 4)
			{
				fprintf(stderr, "Malformed command.\n");
				printUsage();
				return EXIT_FAILURE;
			}

			//key
			if(argv[0][0]=='-' && argv[0][1]=='n' && argv[0][2]=='=')
			{
				//Get key from argv
				int i=0;
				for(i=3; i<strlen(*argv); i++)
				{
					number[i-3] = argv[0][i];
				}
				number[i-3] = '\0';
			}

			//tablefile
			else if(argv[0][0]=='-' && argv[0][1]=='p' && argv[0][2]=='=')
			{
				//Get tablefile from argv
				int i=0;
				for(i=3; i<strlen(*argv); i++)
				{
					primesfileName[i-3] = argv[0][i];
				}
				primesfileName[i-3] = '\0';
			}

			else
			{
				fprintf(stderr, "Malformed command.\n");
				printUsage();
				return EXIT_FAILURE;
			}
		}

		if(strlen(number) == 0 || strlen(primesfileName) == 0)
		{
			fprintf(stderr, "Malformed command.\n");
			printUsage();
			return EXIT_FAILURE;
		}

		performTrialDivision((unsigned char*)number, primesfileName);

		free(number);
		free(primesfileName);
	}

	//millerrabin
	else if(strcmp(*argv, "millerrabin") == 0)
	{
		//Get hold of arguments
		char* number = malloc(100*sizeof(char));
		char* maxIter = malloc(100*sizeof(char));
		char* primesfileName = malloc(100*sizeof(char));

		//No, less or extra parameters for millerrabin
		if(argc-1 < 3 || argc-1 > 3)
		{
			fprintf(stderr, "Malformed command.\n");
			printUsage();
			return EXIT_FAILURE;
		}

		for(argc--; argc>0; argc--)
		{
			argv++;

			if(strlen(*argv) < 4)
			{
				fprintf(stderr, "Malformed command.\n");
				printUsage();
				return EXIT_FAILURE;
			}

			//number
			if(argv[0][0]=='-' && argv[0][1]=='n' && argv[0][2]=='=')
			{
				//Get number from argv
				int i=0;
				for(i=3; i<strlen(*argv); i++)
				{
					number[i-3] = argv[0][i];
				}
				number[i-3] = '\0';
			}

			//primesfile
			else if(argv[0][0]=='-' && argv[0][1]=='p' && argv[0][2]=='=')
			{
				//Get primesfile from argv
				int i=0;
				for(i=3; i<strlen(*argv); i++)
				{
					primesfileName[i-3] = argv[0][i];
				}
				primesfileName[i-3] = '\0';
			}

			//maxiter
			else if(argv[0][0]=='-' && argv[0][1]=='t' && argv[0][2]=='=')
			{
				//Get number from argv
				int i=0;
				for(i=3; i<strlen(*argv); i++)
				{
					maxIter[i-3] = argv[0][i];
				}
				maxIter[i-3] = '\0';
			}

			else
			{
				fprintf(stderr, "Malformed command.\n");
				printUsage();
				return EXIT_FAILURE;
			}
		}

		if(strlen(number) == 0 || strlen(primesfileName) == 0 || strlen(maxIter) == 0)
		{
			fprintf(stderr, "Malformed command.\n");
			printUsage();
			return EXIT_FAILURE;
		}

		runMillerRabin((unsigned char*)number, maxIter, primesfileName);

		free(number);
		free(primesfileName);
		free(maxIter);
	}

	//rndsearch
	else if(strcmp(*argv, "rndsearch") == 0)
	{
		//Get hold of arguments
		char* numbitsString = malloc(100*sizeof(char));
		int numbits = 0;
		char* maxIter = malloc(100*sizeof(char));
		int maxitr = 0;
		char* primesfileName = malloc(100*sizeof(char));
		char* rndfileName = malloc(100*sizeof(char));

		//No, less or extra parameters for millerrabin
		if(argc-1 < 4 || argc-1 > 4)
		{
			fprintf(stderr, "Malformed command.\n");
			printUsage();
			return EXIT_FAILURE;
		}

		for(argc--; argc>0; argc--)
		{
			argv++;

			if(strlen(*argv) < 4)
			{
				fprintf(stderr, "Malformed command.\n");
				printUsage();
				return EXIT_FAILURE;
			}

			//numbits
			if(argv[0][0]=='-' && argv[0][1]=='k' && argv[0][2]=='=')
			{
				//Get numbits from argv
				int i=0;
				for(i=3; i<strlen(*argv); i++)
				{
					numbitsString[i-3] = argv[0][i];
				}
				numbitsString[i-3] = '\0';
			}

			//maxIter
			else if(argv[0][0]=='-' && argv[0][1]=='t' && argv[0][2]=='=')
			{
				//Get maxIter from argv
				int i=0;
				for(i=3; i<strlen(*argv); i++)
				{
					maxIter[i-3] = argv[0][i];
				}
				maxIter[i-3] = '\0';
			}

			//primesfile
			else if(argv[0][0]=='-' && argv[0][1]=='p' && argv[0][2]=='=')
			{
				//Get primesfile from argv
				int i=0;
				for(i=3; i<strlen(*argv); i++)
				{
					primesfileName[i-3] = argv[0][i];
				}
				primesfileName[i-3] = '\0';
			}

			//rndfile
			else if(argv[0][0]=='-' && argv[0][1]=='r' && argv[0][2]=='=')
			{
				//Get rndfile from argv
				int i=0;
				for(i=3; i<strlen(*argv); i++)
				{
					rndfileName[i-3] = argv[0][i];
				}
				rndfileName[i-3] = '\0';
			}

			else
			{
				fprintf(stderr, "Malformed command.\n");
				printUsage();
				return EXIT_FAILURE;
			}
		}

		if(strlen(numbitsString) == 0 || strlen(primesfileName) == 0 || strlen(maxIter) == 0 || strlen(rndfileName)==0)
		{
			fprintf(stderr, "Malformed command.\n");
			printUsage();
			return EXIT_FAILURE;
		}

		numbits = strtod(numbitsString, NULL);
		maxitr = strtod(maxIter, NULL);

		performRndSearch(numbits, maxitr, primesfileName, rndfileName);

		free(numbitsString);
		free(primesfileName);
		free(maxIter);
		free(rndfileName);
	}

	//maurer
	else if(strcmp(*argv, "maurer") == 0)
	{
		//Get hold of arguments
		char* numbitsString = malloc(100*sizeof(char));
		int numbits = 0;
		char* primesfileName = malloc(100*sizeof(char));
		char* rndfileName = malloc(100*sizeof(char));

		//No, less or extra parameters for millerrabin
		if(argc-1 < 3 || argc-1 > 3)
		{
			fprintf(stderr, "Malformed command.\n");
			printUsage();
			return EXIT_FAILURE;
		}

		for(argc--; argc>0; argc--)
		{
			argv++;

			if(strlen(*argv) < 4)
			{
				fprintf(stderr, "Malformed command.\n");
				printUsage();
				return EXIT_FAILURE;
			}

			//numbits
			if(argv[0][0]=='-' && argv[0][1]=='k' && argv[0][2]=='=')
			{
				//Get numbits from argv
				int i=0;
				for(i=3; i<strlen(*argv); i++)
				{
					numbitsString[i-3] = argv[0][i];
				}
				numbitsString[i-3] = '\0';
			}

			//primesfile
			else if(argv[0][0]=='-' && argv[0][1]=='p' && argv[0][2]=='=')
			{
				//Get primesfile from argv
				int i=0;
				for(i=3; i<strlen(*argv); i++)
				{
					primesfileName[i-3] = argv[0][i];
				}
				primesfileName[i-3] = '\0';
			}

			//rndfile
			else if(argv[0][0]=='-' && argv[0][1]=='r' && argv[0][2]=='=')
			{
				//Get rndfile from argv
				int i=0;
				for(i=3; i<strlen(*argv); i++)
				{
					rndfileName[i-3] = argv[0][i];
				}
				rndfileName[i-3] = '\0';
			}

			else
			{
				fprintf(stderr, "Malformed command.\n");
				printUsage();
				return EXIT_FAILURE;
			}
		}

		if(strlen(numbitsString) == 0 || strlen(primesfileName) == 0 || strlen(rndfileName)==0)
		{
			fprintf(stderr, "Malformed command.\n");
			printUsage();
			return EXIT_FAILURE;
		}

		numbits = strtod(numbitsString, NULL);

		performMaurer(numbits, primesfileName, rndfileName);

		free(numbitsString);
		free(primesfileName);
		free(rndfileName);
	}

	else
	{
		fprintf(stderr, "Malformed command.\n");
		printUsage();
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
