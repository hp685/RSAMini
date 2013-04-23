#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<assert.h>
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
		struct ipair r = {t.second, t.first-q*t.second};
		return r;
	}
}

/*int_r to its binary form: Returns a pointer to bits and its size */
/*Returns a reference to a struct*/
/*MAX_INT_LEN is essentially 31 bits*/
struct bit_r* bit_representation(int_r n){

	int sign_bit = 0;
	int i ;
	if(n  <  0 ) { /*Needs a sign bit */
		n = -1 * n;
		sign_bit = 1;
	}

	struct bit_r* t = (struct bit_r*)malloc(sizeof(struct bit_r*));
	/*int sz = (int) ceil(log(n)); /*log(n) bits for an int n */
	char b[MAX_INT_LEN];
	memset(b,0,sizeof(b));
	while(n > 0){

		if(n % 2 == 0)
			strcat(b,"0");
		else
			strcat(b,"1");
		n >>= 1;

	}

	int start = strlen(b);

	for(i = start ; i < MAX_INT_LEN - 1; i++){
		strcat(b,"0");
	}
	/* Sign bit */
	if(sign_bit) strcat(b,"1");
	else strcat(b,"0");
	t->bits = (char*) malloc(sizeof(b)*(MAX_INT_LEN));
	strcpy(t->bits,b);
	t->sz = MAX_INT_LEN;

	return t;
}


int main(){

	struct bit_r* r;
	r = bit_representation(1000);
	printf("%d %s\n", r->sz, r->bits);
	free(r->bits);
	free(r);

	r = bit_representation(-10);
	printf("%d %s\n", r->sz, r->bits);
	free(r->bits);
	free(r);
	return 0;
}
