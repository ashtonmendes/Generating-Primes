hw7: hw7.o primes.o trialdiv.o millerrabin.o rndsearch.o maurer.o utilities.o
	gcc -g -Wall hw7.o primes.o trialdiv.o millerrabin.o rndsearch.o maurer.o utilities.o -L/home/scf-22/csci551b/openssl/lib -lcrypto -lm -o hw7

hw7.o: hw7.c
	gcc -g -Wall -c hw7.c

primes.o: primes.c
	gcc -g -Wall -c primes.c

trialdiv.o: trialdiv.c
	gcc -g -Wall -c trialdiv.c -I/home/scf-22/csci551b/openssl/include

millerrabin.o: millerrabin.c
	gcc -g -Wall -c millerrabin.c

rndsearch.o: rndsearch.c
	gcc -g -Wall -c rndsearch.c

maurer.o: maurer.c
	gcc -g -Wall -c maurer.c

utilities.o: utilities.c
	gcc -g -Wall -c utilities.c -I/home/scf-22/csci551b/openssl/include

clean:
	rm -f *.o hw7
