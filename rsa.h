

#define MAX_BITS 5
/*Allowed to hold larger values later*/
typedef int int_r; /* Prime integer-- 32 bit suffices in our case*/

struct ipair{
	int_r first, second;
ipair(int_r i, int_r j) : first(i), second(j);
};

struct bit_r{

	char* bits;
	int sz; /*Number of bits*/
}
bool isprime(const int_r& n); /* Miller-Rabin Primality testing */
int generateRandomNumber();  /* Wrapper to the C rand() */
int_r fastModularExponentiation(int_r base, int_r exponent);
ipair eea_gcd(int_r a, int_r b); /* Extended Euclidean Algorithm */
bit_r bit_representation(int_r n);
