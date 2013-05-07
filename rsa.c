#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<assert.h>
#include<time.h>
#include"rsa.h"


#define TEST
#undef TEST

static int seed = 0;
/*May need a sophisticated random generator later*/
int generateRandomBits(){

	int r = rand();
	return r;
}

/*In class version of Miller-Rabin*/
/*Eveything modulo m */
int isPrime(const int_r base, const int_r exponent){

	int_r y = 1;
	int_r k = 1 + (int)ceil(log(exponent));
	/*Disregard the other bits in bit representation*/
	struct bit_r* exp_bits = bit_representation(exponent-1);
#ifdef DEBUG
	printf("Binary Representation of the exponent: %s\n", exp_bits->bits);
#endif
	int i;

	for( i = k; i > 0; i--){
		int_r z = y;
		y = (y * y) % exponent;
#ifdef DEBUG
		printf("i:%d x:%c y:%d z:%d\n", MAX_INT_LEN - i , exp_bits->bits[i], y, z);
#endif
		if( y == 1 && z != 1 && z != (exponent - 1) ){
			return 0;
		}


		if(exp_bits->bits[MAX_INT_LEN - i] == '1'){
			y =  y *  base % exponent ;
		}
	}
	/*Free bit_r*/
	free(exp_bits->bits);
	free(exp_bits);
	return (y == 1) ? 1 : 0;

}

/*Limit to this file*/
/*In place*/
static
char* reverse(char* s, int sz){

	int i;
	for(i = 0; i < (sz>>1); i++){
		char t  = s[i];
		s[i] = s[sz - i - 1];
		s[sz - i - 1] = t;
	}
	return s;
}

/*Extended euclidean algorithm for multiplicative inverses*/
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
/*MAX_INT_LEN is essentially 31 bits -- 0 thru 30 */
struct bit_r* bit_representation(int_r n){

	char b[MAX_INT_LEN];
	int sign_bit = 0;
	int i ;
	if(n  <  0 ) { /*Needs a sign bit */
		n = -1 * n;
		sign_bit = 1;
	 }

	 struct bit_r* t = (struct bit_r*)malloc(sizeof(struct bit_r*));
	 /*int sz = (int) ceil(log(n)); /*log(n) bits for an int n */

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
	 /*Allocate space for the bits*/
	 t->bits = (char*) malloc(sizeof(b)*(MAX_INT_LEN));
	 strcpy(t->bits,b);
	 t->sz = MAX_INT_LEN;
	 t->bits = reverse(t->bits, t->sz);
	 return t;
 }



 /* Generates candidate primes according to the spec*/
 struct bit_r* generateRandomNumbers(){

	 /*Pad the leading bits*/

	 char *prefix = "00000000000000000000000001";

	 int i;

	 struct bit_r *numbers = (struct bit_r*) malloc(sizeof(struct bit_r));

	 // memset(numbers->bits, '\0',sizeof(numbers->bits));
	 numbers->bits = (char*) malloc(sizeof(char) * MAX_INT_LEN + 1);

	 strcpy(numbers->bits, prefix);

	 srand(seed++);

	 for(i = 0; i < MAX_BITS; i++){
		 int a  = generateRandomBits() % 2;

		 if(a == 1)
			 strcat(numbers->bits,"1");
		 else
			 strcat(numbers->bits,"0");


	 }

	strcat(numbers->bits,"1");
	numbers->sz   = strlen(numbers->bits);

	return numbers;
}

/*Takes a stream of bits and outputs an integer*/
int_r bits_to_int_r(struct bit_r* numbers ){

	int i = 0;
	int_r n = 0;

	//printf("%d\n",strlen(numbers->bits));

	for(i = strlen(numbers->bits) - 1; i >= 0; i--){

		if(numbers->bits[i] == '1' ){
			n += (1 << (strlen(numbers->bits)- 1 - i));
			//printf("%d\n",number);
		}

	}

	return n;
}


/*Check with twenty values of a*/
/*Should always return a prime(whp)*/
int_r candidate_primes(){

	int i;
	struct bit_r *p = generateRandomNumbers();

	int_r x = bits_to_int_r(p);
	int pass = 0;

	for(i = 0; i < NUM_CHECKS; i++){
		p = generateRandomNumbers();
		int_r a = bits_to_int_r(p)%(x-1);
#ifdef DEBUG
		printf("%d %d\n",a,x);
#endif
		pass = isPrime(a,x);

		if(!pass)
			break;
	}
	free(p->bits);
	free(p);
	return pass == 1 ? x : candidate_primes();
}


#ifdef TEST
int main(){

	/*TESTS*/
	struct bit_r* r;
	r = bit_representation(1000);
	printf("%d %s\n", r->sz, r->bits);
	free(r->bits);
	free(r);

	r = bit_representation(-10);
	printf("%d %s\n", r->sz, r->bits);
	free(r->bits);
	free(r);

	struct ipair eg = eea_gcd(10,15);
	printf("%d %d\n", eg.first, eg.second);

	/* char *name = "Harsh"; */
	/* char *nname = (char*)malloc(sizeof(name)); */
	/* strcpy(nname, name);/\*Mutability*\/ */

	//char *ret = reverse(nname, strlen(name));
	//printf("%s\n", ret);
	//free(nname);

	int b = isPrime(3, 2); /*returns true*/
	assert(b == 1);

	int nb = isPrime(2, 27);
	assert(nb == 0);

	int i = 0;
	/*Random bit test*/
	/* for( i = 0; i < 20; i++){ */
	/* 	printf("%d\n",generateRandomBits()); */
	/* } */

	/*Generate Random Numbers Test*/

	//struct bit_r* rn = generateRandomNumbers();
	//printf("Random Number bits %s, %d\n", rn->bits, rn->sz);
	//printf("Bits to Int: %d\n", bits_to_int_r(rn));

	for(i = 0; i < 20; i++){
		int_r prime = candidate_primes();
		printf("%d\n",prime);
	}

	/* For(i = 0 ; i < 20; i++){ */
	/* 	srand(getpid()); */
	/* 	struct bit_r* rn = generateRandomNumbers(); */

	/* 	printf("Random Number bits %s, %d\n", rn->bits, rn->sz); */
	/* 	free(rn->bits); */
	/* 	free(rn); */

	/* } */



	return 0;


}
#endif
