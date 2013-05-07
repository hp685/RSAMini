#ifndef RSA_H
#define RSA_H
#endif

#define MAX_BITS 5
#define MAX_INT_LEN 32
#define NUM_CHECKS 20
/*Allowed to hold larger values later*/
typedef int int_r; /* Prime integer-- 32 bit suffices in our case*/

struct ipair{
	int_r first, second;
};

struct bit_r{
	char* bits;
	int sz; /*Number of bits*/
};
int isPrime(const int_r, const int_r); /* Miller-Rabin Primality testing */

struct bit_r* generateRandomNumbers();  /* Wrapper to the C rand() */

struct ipair eea_gcd(int_r , int_r ); /* Extended Euclidean Algorithm */
struct bit_r* bit_representation(int_r );
int_r bits_to_int_r(struct bit_r*);
int_r candidate_primes();
