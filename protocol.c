#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<assert.h>
#include "rsa.h"
#include "certificate.h"

#define TRACE

/*Euclidean gcd*/
static
int gcd(int_r a, int_r b){

	if(b == 0)
		return a;
	else
		return gcd(b, a % b);
}


int_r fast_exponentiation(int_r a, int_r b, int_r c){

	int_r y = 1;
	struct bit_r *x = bit_representation(b);

	int i;

	for(i = strlen(x->bits); i >= 0; i--){
		if(x->bits[i] == '1'){
			y = y * y % c;
		}
	}

	free(x->bits);
	free(x);
	return y;
}

/*e, d, phi_n, p, q*/
struct
key_pair* generate_key_pair(){

	/*Public Key generation*/
	/* (e,d) should be relatively prime with phi(n) */

	int i ;
	int is_coprime = 0;

	struct key_pair *kp = (struct key_pair*) malloc(sizeof(struct key_pair));

	for( i = 3; ;i++){ /*Generate e*/

		struct prime_pair *pp = generate_prime_pair();
		/*Needs a recursive structure here: When i exceeds phi_n*/

		int is_coprime = gcd(i, pp->phi_n);
		if(is_coprime){
			/*Find the multiplicative inverse d*/
			struct ipair r = eea_gcd(pp->phi_n, i);

			kp->e     = i;
			kp->d     = r.second;
			kp->phi_n = pp->phi_n;
			kp->p     = pp->p;
			kp->q     = pp->q;

			free(pp);
			if(kp->e == kp->d)
				continue;
			/* Bad idea to have e == d */
			assert( kp->e != kp->d );

			return kp;
		}

	}
}


/* (p, q, n)  */
struct
prime_pair* generate_prime_pair(){

	int_r p = candidate_primes();
	int_r q = candidate_primes();
	int_r phi_n = (p-1) * (q - 1);

	struct prime_pair *pp = (struct prime_pair*) malloc(sizeof(struct prime_pair));
	pp->p = p;
	pp->q = q;
	pp->phi_n = phi_n;

#ifdef TRACE
	printf("p:%d q:%d p*q:%d phi_n:%d \n", p, q, p * q, phi_n);
#endif

	return pp;
}





/* R -- Pair <Person, Public Key>
   S -- Signature

  R Spec:

  Bytes 1-6 : Name
  Bytes 7-10: n padded with leading zero bits
  Bytes 11-14: e padded with leading zero bits

  S Spec:

  Partition R into one byte blocks and compute XOR
  which will be the hash value

 */


void create_certificate(struct person *p1, struct person *p2){

	/*R*/
	printf("IN Create Certificate\n");
	int i;

	struct certificate* cf = (struct certificate*) malloc(sizeof(struct certificate));
	char *cr;

	/*Bit representation of n*/

	struct bit_r *bn = bit_representation(p1->kp->p * p1->kp->q);
	struct bit_r *be = bit_representation(p1->kp->e);
	strcat(cr,p1->name);
	strcat(cr,bn->bits);
	strcat(cr,be->bits);

	/*Create a certificate*/
	cf->r = cr;
	char *current = '\0';
	/*Compute h(r)*/
	for(i = 1; i <= strlen(cf->r); i++){

		strcat(current,(char*)cf->r[i]);
		if(i % 8 == 0){
			printf("%d\n",cf->hs);
			cf->hs ^= bits_to_int_r(current);
			current = '\0';
		}
	}
	/*S*/
	/**Signing*/

	/*Should be 8 bits*/
	assert(strlen(cf->hs) == 8);

	cf->signature = fast_exponentiation(cf->hs, p2->kp->e, p2->kp->p * p2->kp->q);



}




int main(){


	int i;
	for(i = 0; i < 20; i++){
		int_r prime = candidate_primes();
		printf("%d\n",prime);
	}

	struct person *p1 = (struct person*)malloc(sizeof(struct person));
	/*Alice*/

	p1->name = (char*) malloc(sizeof(char) * 50);
	p1->name = "0000000000100000101101100011010010110001101100101";

	p1->kp   = generate_key_pair();



	struct person *p2 = (struct person*)malloc(sizeof(struct person));
	/*Trent*/
	p2->name = "000000000101010001110010011001010110111001110100";
	p2->kp   = generate_key_pair();

	/*Signing*/
	create_certificate(p1, p2->kp->d);


}
