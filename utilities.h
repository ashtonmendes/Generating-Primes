#ifndef UTILITIES_H_
#define UTILITIES_H_

void set(unsigned char* , unsigned int );
void clear(unsigned char* , unsigned int );
unsigned char check(unsigned char*, unsigned int n);
char* Eratosthenes(unsigned int, unsigned char*);
unsigned int* readPrimesFile(char*, int*);
int trialDivision(unsigned char*, unsigned int* , unsigned int, int, int);
void printSpaces(int );
char* millerRabin(unsigned char*, int, unsigned int*, int, int, int, int);
unsigned char* RndOddNum(int , FILE*);
unsigned char* randomSearch(int , int , char* , FILE* );
unsigned char rndByte(FILE*);
unsigned char* Maurer(int , char* , FILE* , int);

#endif /* UTILITIES_H_ */
