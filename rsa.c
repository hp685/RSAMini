
#include<stdlib.h>
#include"rsa.h"

bool isprime(const int_r& n){


}

/*May need a sophisticated random generator later*/
int generateRandomNumber(){

	int r = rand() % MAX_BITS;
	return r;
}


int_r fastModularExponentiation(int_r base, int_r exponent, int_r m){


}


struct ipair eea_gcd(int_r a, int_r b){

	if(b == 0)
		return ipair(1,0);
	else{
		q = a / b;
		r = a % b;
		ipair t = eea_gcd(b,r);
		return( t.second, t.first - q * t.second );
	}
}

/*int_r to its binary form: Returns a pointer to bits and its size */
/*Returns a reference to a struct*/
struct bit_r bit_representation(int_r n){

	bit_r t;
	int sz = (int) ceil(log(n)); /*log(n) bits for an int n */
	char b[sz + 1];
	while(n > 0){

		if(n % 2 == 0)
			strcat(b,0);
		else
			strcat(b,1);
		n /= 2;

	}
	t.bits = (char*) malloc(sizeof(b)*(sz+1));
	strcpy(t.bits,b);
	t.sz = sz+1;

	return t;
}
