#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<openssl/bn.h>
#include<netinet/in.h>

#define round(X) (((X) >= 0) ? (int)((X)+0.5) : (int)((X)-0.5))

void set(unsigned char* array, unsigned int n) //n is 1-based index
{
	int bucketNum = n/8;
	int bitNum = (n%8) + 1; //1-based index in byte

	unsigned char mask = (unsigned char)1 << (8-bitNum); //single bit to set

	array[bucketNum] = array[bucketNum] | mask;
}

void clear(unsigned char* array, unsigned int n)
{
	int bucketNum = n/8;
	int bitNum = (n%8) + 1; //1-based index in byte

	unsigned char mask = ~((unsigned char)1 << (8-bitNum)); //single bit is 0

	array[bucketNum] = array[bucketNum] & mask;
}

unsigned char check(unsigned char* array, unsigned int n)
{
	int bucketNum = n/8;
	int bitNum = (n%8) + 1; //1-based index in byte

	unsigned char mask = (unsigned char)1 << (8-bitNum);

	return (array[bucketNum] & mask)==mask;
}

void Eratosthenes(unsigned int n, unsigned char* array)
{
	int numOfBytes = ceil((float)n/(float)8); //how many bytes required

	//initialize
	int i=0;
	for(i=0; i<numOfBytes; i++)
		array[i]=0;

	clear(array, 1); //1 is not a prime

	for(i=2; i<=n; i++) //initially all numbers from 2 to n are considered primes
		set(array, i);

	unsigned int p = 2;
	unsigned int j = 0;

	/*printf("Initially ");
	int z = 2;
	for(z=2; z<=n; z++)
		printf("a[%d]=%d ", z, check(array, z));
	printf("\n");*/

	while( p*p <= n)
	{
		j = p*p;

		//printf("p=%d\n", p);

		while(j <= n)
		{
			clear(array, j);
			j = j + p;
		}

		/*printf("After clearing multiples of %d, ", p);
		int z = 2;
		for(z=2; z<=n; z++)
			printf("a[%d]=%d ", z, check(array, z));
		printf("\n");*/

		do
		{
			p++;
			//printf("check %d = %d\n", p, check(array, p));
		}
		while(check(array, p)!=1); //skips those numbers who are already marked as composite
	}
}

unsigned int* readPrimesFile(char* primesfileName, int* sizeOfNumbers)
{
	FILE *file = fopen(primesfileName, "rb");
	if(strlen(primesfileName)!=0 && file==NULL) //Check for IO errors
	{
		fprintf(stderr, "Unable to open %s.\n", primesfileName);
		return NULL;
	}

	//Find size of file
	fseek(file, 0L, SEEK_END);
	int size = ftell(file);
	fseek(file, 0L, SEEK_SET);

	//size should be a multiple of 4 (32-bit word)
	if((size % 4) != 0)
	{
		fprintf(stderr, "%s is not a multiple of 4 bytes. Hence it is an invalid primesfile.\n", primesfileName);
		return NULL;
	}

	unsigned int* numbers = (unsigned int*)malloc((size/4) * sizeof(unsigned int));

	*sizeOfNumbers = (size/4);

	int bytesRead = -1;
	unsigned char buffer[4];

	int index = 0, i=0;

	unsigned int temp = 0;

	//Read 4 bytes at a time
	while(bytesRead != 0)
	{
		bytesRead = fread(buffer, sizeof(buffer), 1, file);

		if(bytesRead == 0) break;

		//1 word has been read, onvert it to int and transfer to 'numbers' array
		temp = 0;
		for(i=0; i<4; i++)
			temp = (temp<<8) | buffer[i];

		numbers[index++] = temp;

		//printf("found %d\n", numbers[index-1]);
	}

	fclose(file);
	return numbers;
}

void printSpaces(int n)
{
	int i=0;
	for(i=0; i<n; i++)
		printf(" ");
}

int trialDivision(unsigned char* dividendString, unsigned int* numbersFromPrimesFile, unsigned int sizeOfNumbers, int printResults, int initialSpacing)
{
	BIGNUM* dividend = BN_new();
	BN_dec2bn(&dividend, (char*)dividendString); //convert number to BIGNUM representation

	//TRIAL DIVISION
	int i=0;
	BIGNUM* divisor = BN_new();
	BIGNUM* remainder = BN_new();
	BN_CTX* bn_ctx = BN_CTX_new();

	BIGNUM* divisorSqr = BN_new();

	int result = 1;

	for(i=1; i<sizeOfNumbers; i++)
	{
		divisor = BN_new();
		remainder = BN_new();
		bn_ctx = BN_CTX_new();

		BN_bin2bn((unsigned char *)(&numbersFromPrimesFile[i]), sizeof(uint32_t), divisor); //prime number from primesfile

		//Need to check if divisor^2 <= number
		BN_sqr(divisorSqr, divisor, bn_ctx);
		if(BN_cmp( divisorSqr, dividend) == 1) //divisor^2 <= number is not true, break
			break;

		BN_mod(remainder, dividend, divisor, bn_ctx); //perform number % (primesfile number)

		/*printf(BN_bn2dec(dividend));
		printf(" mod ");
		printf(BN_bn2dec(divisor));
		printf(" = ");
		printf(BN_bn2dec(remainder));
		printf(" isZero: ");
		printf("%d", BN_is_zero(remainder));
		printf("\n");*/

		if(BN_is_zero(remainder) == 1) //number is divisible by a prime number from primesfile
		{
			result = 0; //trial division failed

			if(printResults)
			{
				printSpaces(initialSpacing);
				printf("n is composite by trial division (mod %s = 0)\n", BN_bn2dec(divisor));
			}

			break;
		}
	}

	//convert maxval to BIGNUM
	BIGNUM* maxval = BN_new();
	BIGNUM* maxvalSqr = BN_new();
	BN_bin2bn((unsigned char *)(&numbersFromPrimesFile[0]), sizeof(uint32_t), maxval);
	BN_sqr(maxvalSqr, maxval, bn_ctx);

	if(result == 1) //trial division succeeded.
	{
		if(BN_cmp( maxvalSqr, dividend) == -1) //If the maxval in primesfile is strictly less than floor(sqrt(number)), i.e maxval^2 < number
		{
			if(printResults)
			{
				printSpaces(initialSpacing);
				printf("n passes trial division test (not enough primes)\n");
			}
		}
		else
		{
			if(printResults)
			{
				printSpaces(initialSpacing);
				printf("n passes trial division test\n");
			}
		}
	}

	BN_free(dividend);
	BN_free(divisor);
	BN_free(remainder);
	BN_free(divisorSqr);
	BN_free(maxval);
	BN_free(maxvalSqr);
	BN_CTX_free(bn_ctx);

	return result;
}

char* millerRabin(unsigned char* numberString, int maxitr, unsigned int* primeNumbers,int sizeOfNumbers, int printResults, int initialSpacing, int printN)
{
	//maxitr is too large or that there is not enough primes in primesfile
	if(maxitr > sizeOfNumbers-1)
	{
		if(printResults)fprintf(stderr, "primesfile contains %d only prime numbers. Hence cannot perform %d iterations.\n", (sizeOfNumbers-1), maxitr);
		return "failure";
	}

	BIGNUM* n = BN_new();
	BN_dec2bn(&n, (char*)numberString); //convert numberString to BIGNUM representation

	BIGNUM* one = BN_new(); BN_one(one);
	BIGNUM* nMinus1 = BN_new();
	BN_sub(nMinus1, n, one);

	int s = 0;
	BIGNUM* r = BN_new();
	BN_copy(r, nMinus1);

	//printf("n=%s\n", BN_bn2dec(r));

	BIGNUM* remainder = BN_new();
	BIGNUM* two = BN_new();
	uint32_t intTwo = 2;
	BN_bin2bn((unsigned char *)(&intTwo), sizeof(uint32_t), two);
	BN_CTX* bn_ctx = BN_CTX_new();
	BN_mod(remainder, r, two, bn_ctx); //remainder = r%2

	//split n into 2 parts - > 2^s and r such that r is odd.
	while(BN_is_zero(remainder) && (BN_is_zero(r) != 1)) //while r%2==0 and r!=0
	{
		BN_rshift1(r, r);
		s++;
		BN_mod(remainder, r, two, bn_ctx);
	}

	//printf("s=%d r=%s\n",s,  BN_bn2dec(r));

	int i=1, j=1;
	BIGNUM* a = BN_new();
	BIGNUM* y = BN_new();

	//print n, n-1, s and r
	if(printResults)
	{
		if(printN)
		{
			printSpaces(initialSpacing);
			printf("n = %s\n", BN_bn2dec(n));
		}
		printSpaces(initialSpacing);
		printf("  n-1 = %s\n", BN_bn2dec(nMinus1));
		printSpaces(initialSpacing);
		printf("  s = %d\n", s);
		printSpaces(initialSpacing);
		printf("  r = %s\n", BN_bn2dec(r));
	}

	for(i=1; i<=maxitr; i++)
	{
		// a = primeNumbers[i]
		a = BN_new();
		BN_bin2bn((unsigned char *)(&primeNumbers[i]), sizeof(uint32_t), a);

		nMinus1 = BN_new();
		BN_sub(nMinus1, n, one);

		//if (a > n-1)
		if(BN_cmp(a, nMinus1) == 1)
		{
			if(printResults)fprintf(stderr, "Failure. Because a > n-1, i.e. %s > %s\n", BN_bn2dec(a), BN_bn2dec(nMinus1));
			return "failure";
		}

		//compute y = a^r mod n
		y = BN_new();
		BN_mod_exp(y, a, r, n, bn_ctx);

		if(printResults)
		{
			printSpaces(initialSpacing);
			printf("  Itr %d of %d, a = %s, y = %s", i, maxitr, BN_bn2dec(a), BN_bn2dec(y));
			if(BN_cmp(y, nMinus1)==0)
				printf(" (which is n-1)");
			printf("\n");
		}

		//if (y != 1 and y != n-1)
		if(BN_is_one(y)!=1 && BN_cmp(y, nMinus1)!=0)
		{
			for(j=1; j<=(s-1) && BN_cmp(y, nMinus1)!=0; j++)
			{
				BN_mod_sqr(y, y, n, bn_ctx); //y = y^2 mod n

				if(printResults)
				{
					printSpaces(initialSpacing);
					printf("    j = %d of %d, y = %s", j, (s-1), BN_bn2dec(y));
					if(BN_cmp(y, nMinus1)==0)
						printf(" (which is n-1)");
					printf("\n");
				}

				//if (y == 1)
				if(BN_is_one(y) == 1)
				{
					if(printResults)
					{
						printSpaces(initialSpacing);
						printf("Miller-Rabin found a strong witness %s\n", BN_bn2dec(a));
					}
					return "composite";
				}
			}

			//if (y != n-1)
			if(BN_cmp(y, nMinus1)!=0)
			{
				if(printResults)
				{
					printSpaces(initialSpacing);
					printf("Miller-Rabin found a strong witness %s\n", BN_bn2dec(a));
				}
				return "composite";
			}
		}
	}

	if(printResults)
	{
		printSpaces(initialSpacing);
		printf("Miller-Rabin declares n to be a prime number\n");
	}

	BN_free(n);
	BN_free(one);
	BN_free(nMinus1);
	BN_free(r);
	BN_free(remainder);
	BN_free(two);
	BN_CTX_free(bn_ctx);
	BN_free(a);
	BN_free(y);

	return "prime";
}

unsigned char* RndOddNum(int k, FILE *rndfile)
{
	int x = ceil((float)k/(float)8); //how many bytes to read.

	unsigned char buffer[x];

	BIGNUM* randomNum = BN_new();
	BN_zero(randomNum);

	int totalBits = 0, currentBit = -1;

	int bytesRead = fread(buffer, sizeof(buffer), 1, rndfile); //read next x bytes from file
	if(bytesRead != 0)
	{
		//convert byte array to BIGNUM representation
		BN_bin2bn(buffer, x, randomNum);

		//set bit 0 and bit k-1 to 1
		BN_set_bit(randomNum, 0);
		BN_set_bit(randomNum, k-1);

		//sets all bits from k and above, to 0 in the BIGNUM
		totalBits = 8 * BN_num_bytes(randomNum);
		for(currentBit=k; currentBit<totalBits; currentBit++)
			BN_clear_bit(randomNum, currentBit);
	}

	unsigned char* randomNumString = (unsigned char*)BN_bn2dec(randomNum);

	BN_free(randomNum);

	return randomNumString;
}


/**
 * Random-Search(k,maxitr):
    1) n = RndOddNum(k)
    2) if (TrialDivision(n) == "fail") goto step (1)
    3) if (Miller-Rabin(n,maxitr) == "prime") return(n)
       goto step (1)
 */
unsigned char* randomSearch(int k, int maxitr, char* primesfileName, FILE* rndfile)
{
	int size;
	unsigned int* numbersFromPrimesFile = readPrimesFile(primesfileName, &size);

	unsigned char* numberString;
	unsigned int iterNum = 1;
	while(1)
	{
		printf("RANDOM-SEARCH: iteration %d\n", iterNum++);

		numberString = (unsigned char*)RndOddNum(k, rndfile);

		printf("  n = %s\n", numberString);

		if(trialDivision(numberString, numbersFromPrimesFile, (unsigned int)size, 1, 2) == 0) //trial division failed
			continue;

		if(strcmp(millerRabin(numberString, maxitr, numbersFromPrimesFile, size, 1, 2, 0), "prime")==0)
			return numberString;
	}
}

unsigned char rndByte(FILE *rndfile)
{
	unsigned char buffer[1];

	fread(buffer, sizeof(buffer), 1, rndfile); //read 1 byte

	return buffer[0];
}

/*
 * Maurer(k):
    1) if (k <= 20) do forever {
       1.1) n = RndOddNum(k)
       1.2) if (TrialDivision(n) == "pass") return n
       }
    2) c = 0.1, m = 20
    3) B = c * k^2 (not used)
    4) (generate a fraction r, the size of q relative to n)
       4.1) if (k <= 2m) r = 0.5
       4.2) if (k > 2m) do forever {
            4.2.1) r = RndByte() / 255.0
            4.2.2) r = 0.5 + r / 2.0
            4.2.3) if (k*(1-r) > m) break;
            }
    5) (recursion) q = Maurer(floor(r*k)+1)
    6) num_bits_in_q = BN_num_bits(q)
       I = floor(2^(k-2) / q)
    7) do forever {
       7.1) R = RndOddNum(k+1-num_bits_in_q)
            R = (R mod I) + I + 1
            n = 2Rq+1
       7.2) if (TrialDivision(n) != "fail") {
                   num_bits_in_n = BN_num_bits(n)
            7.2.1) do {
                       a = RndOddNum(num_bits_in_n)
                   } while (a <= 1 or a >= n-1))
            7.2.2) b = a^{n-1} mod n
                   if (b == 1) {
                       b = a^{2R} mod n
                       d = gcd(b-1,n)
                       if (d == 1) return(n)
                   }
            }
       }
 */
unsigned char* Maurer(int k, char* primesfileName, FILE* rndfile, int level)
{
	printf("Maurer: level %d, k=%d\n", level, k);

	unsigned char* n;
	unsigned char* qString;
	unsigned char* Rstring;
	unsigned char* aString;
	unsigned char rndByteReturn = 0;
	int size;
	int num_bits_in_q = 0, num_bits_in_n = 0, iterNum = 0;
	unsigned int* numbersFromPrimesFile = readPrimesFile(primesfileName, &size);
	double c = 0, B = 0, r = 0;
	int m = 0;
	BIGNUM* q = BN_new();
	BIGNUM* two = BN_new();
	BIGNUM* kMinus2 = BN_new();
	BIGNUM* I = BN_new();
	BN_CTX* bn_ctx = BN_CTX_new();
	BIGNUM* R = BN_new();
	BIGNUM* one = BN_new();
	BIGNUM* bn_n = BN_new();
	BIGNUM* a = BN_new();
	BIGNUM* nMinus1 = BN_new();
	BIGNUM* b = BN_new();
	BIGNUM* temp = BN_new();
	BIGNUM* d = BN_new();
	BN_one(one);
	uint32_t tempInt = 2;
	BN_bin2bn((unsigned char *)(&tempInt), sizeof(uint32_t), two);
	tempInt = k-2;
	BN_bin2bn((unsigned char *)(&tempInt), sizeof(uint32_t), kMinus2);

	if(k <= 20)
	{
		while(1)
		{
			n = RndOddNum(k, rndfile);

			printf("  step 1.1, n = %s\n", n);

			if(trialDivision(n, numbersFromPrimesFile, size, 1, 4) == 1) //trial division passed
			{
				BN_dec2bn(&bn_n, (char*)n);
				if(level==0)printf("\nMaurer's Algorithm found an %d-bit prime:\n  n = %s\n", BN_num_bits(bn_n), n);
				return n;
			}
		}
	}

	c = 0.1; m = 20;

	B = c * k * k;

	//generate a fraction r, the size of q relative to n
	printf("  step 4");
	if(k <= 2*m)
		r = 0.5;
	if(k > 2*m)
	{
		while(1)
		{
			rndByteReturn = rndByte(rndfile);
			r = rndByteReturn / 255.0;
			r = 0.5 + r / 2.0;
			if( (k*(1-r)) > m)
			{
				printf(": random byte = %d", rndByteReturn);
				break;

			}
		}
	}
	printf(", r = %d%%\n", round(100.0*r));

	//RECURSION
	qString = Maurer(floor(r*k)+1, primesfileName, rndfile, level+1);
	BN_dec2bn(&q, (char*)qString); //convert q to BIGNUM representation
	printf("Maurer: back to level %d, k=%d, q=%s\n", level, k, qString);

	num_bits_in_q = BN_num_bits(q);

	//I = floor(2^(k-2) / q)
	BN_exp(I, two, kMinus2, bn_ctx);
	BN_div(I, NULL, I, q, bn_ctx);

	while(1)
	{
		iterNum++;

		//R = RndOddNum(k+1-num_bits_in_q)
		Rstring = RndOddNum(k+1-num_bits_in_q, rndfile);
		BN_dec2bn(&R, (char*)Rstring);

		//R = (R mod I) + I + 1
		BN_mod(R, R, I, bn_ctx);
		BN_add(R, R, I);
		BN_add(R, R, one);

		//n = 2Rq+1
		BN_mul(bn_n, two, R, bn_ctx);
		BN_mul(bn_n, bn_n, q, bn_ctx);
		BN_add(bn_n, bn_n, one);
		n = (unsigned char*)BN_bn2dec(bn_n);

		printf("  step 7, itr %d: R = %s, n = %s\n", iterNum, BN_bn2dec(R), n);

		if(trialDivision(n, numbersFromPrimesFile, size, 1, 4) == 1) //trial division passes
		{
			num_bits_in_n = BN_num_bits(bn_n);

			do
			{
				aString = RndOddNum(num_bits_in_n, rndfile);
				BN_dec2bn(&a, (char*)aString);

				BN_sub(nMinus1, bn_n, one); //n-1
			}
			while(BN_cmp(a, one)<=0 || BN_cmp(a, nMinus1)>=0 ); //while (a <= 1 or a >= n-1))

			printf("  step 7.2.1, itr %d: a = %s\n", iterNum, aString);

			//b = a^{n-1} mod n
			BN_mod_exp(b, a, nMinus1, bn_n, bn_ctx);

			if(BN_is_one(b))
			{
				//b = a^{2R} mod n
				BN_mul(temp, two, R, bn_ctx);
				BN_mod_exp(b, a, temp, bn_n, bn_ctx);

				//d = gcd(b-1,n)
				BN_sub(temp, b, one); //b-1
				BN_gcd(d, temp, bn_n, bn_ctx);

				if(BN_is_one(d))
				{
					if(level==0)printf("\nMaurer's Algorithm found an %d-bit prime:\n  n = %s\n", BN_num_bits(bn_n), n);
					return n;
				}
			}
		}
	}

	BN_free(q);
	BN_free(two);
	BN_free(kMinus2);
	BN_free(I);
	BN_CTX_free(bn_ctx);
	BN_free(R);
	BN_free(one);
	BN_free(bn_n);
	BN_free(a);
	BN_free(nMinus1);
	BN_free(b);
	BN_free(temp);
	BN_free(d);
}






