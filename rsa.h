#ifndef RSA_H
#define RSA_H
#endif

#define MAX_BITS 5
#define MAX_INT_LEN 32
/*Allowed to hold larger values later*/
typedef int int_r; /* Prime integer-- 32 bit suffices in our case*/

struct ipair{
	int_r first, second;
};

struct bit_r{
	char* bits;
	int sz; /*Number of bits*/
};
int isprime(const int_r*); /* Miller-Rabin Primality testing */
int generateRandomNumber();  /* Wrapper to the C rand() */
int_r fastModularExponentiation(int_r, int_r, int_r);
struct ipair eea_gcd(int_r , int_r ); /* Extended Euclidean Algorithm */
struct bit_r bit_representation(int_r );
