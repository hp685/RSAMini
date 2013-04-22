
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include"rsa.h"

int isprime(const int_r* n){


}

/*May need a sophisticated random generator later*/
int generateRandomNumber(){

	int r = rand() % MAX_BITS;
	return r;
}


 int_r fastModularExponentiation(int_r base, int_r exponent, int_r m){


}


struct ipair eea_gcd(int_r a, int_r b){

	if(b == 0){
		struct ipair t;
		t.first = 1;
		t.second = 0;
		return t;

	}
	else{
		int_r q  = a / b;
		int_r re  = a % b;
		struct ipair t = eea_gcd(b,re);
		struct ipair r;
		r.first  = t.second;
		r.second = t.first - q*t.second;
		return r;
	}
}

/*int_r to its binary form: Returns a pointer to bits and its size */
/*Returns a reference to a struct*/
struct bit_r bit_representation(int_r n){

	struct bit_r t;
	int sz = (int) ceil(log(n)); /*log(n) bits for an int n */
	char b[sz + 1];
	while(n > 0){

		if(n % 2 == 0)
			strcat(b,"0");
		else
			strcat(b,"1");
		n /= 2;

	}
	t.bits = (char*) malloc(sizeof(b)*(sz+1));
	strcpy(t.bits,b);
	t.sz = sz+1;

	return t;
}
