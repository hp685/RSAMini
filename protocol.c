#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<assert.h>
#include"rsa.h"


/* Generates candidate primes according to the spec*/
struct bit_r* generateRandomNumbers(){

	/*Pad the leading bits*/
	char *prefix = '00000000000000000000001';
	int i;
	struct bit_r *numbers = (struct bit_r*) malloc(sizeof(struct bit_r*));
	numbers->bits = (char*) malloc(sizeof(char* ) * MAX_INT_LEN);

	for(i = 0; i < MAX_BITS; i++){

		char bit = itoa(generateRandomBits());
		strcat(prefix,bit);
	}

	strcat(prefix,'1');
	numbers->bits = prefix;
	numbers->sz   = MAX_INT_LEN;
	return numbers
}

/*Takes a stream of bits and outputs an integer*/
int_r bits_to_int_r(struct bit_r* numbers ){

	int i;
	int_r number = 0;
	for(i = 0; i < strlen(numbers->bits); i++){

		if(numbers->bits[i] == '1' ){
			number <<= 1;
		}
	}

	return number;
}

/*Check with twenty values of a*/
int_r candidate_primes(){
	int i;


	for(i = 0; i < NUM_CHECKS; i++){

		int_r a = bits_to_int_r(generateRandomNumbers());

	}
}


int main(){


}
