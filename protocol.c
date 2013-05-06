#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<assert.h>
#include"rsa.h"
#include "certificate.h"

/*Euclidean gcd*/
static
int gcd(int_r a, int_r b){

	if(b == 0)
		return a;
	else
		return gcd(b, a % b);
}


/*e, d, phi_n, p, q*/
struct
key_pair* generate_key_pair(){

	/*Public Key generation*/
	/* (e,d) should be relatively prime with phi(n) */

	int i ;
	int is_coprime = 0;

	struct keypair *kp = (struct key_pair*) malloc(sizeof(struct key_pair));
	for( i = 3; ;i++){ /*Generate e*/

		struct prime_pair *pp = generate_prime_pair();

		int is_coprime = gcd(i, pp->phi_n);
		if(is_coprime){
			/*Find the multiplicative inverse d*/
			struct ipair r = eea_gcd(phi_n, i);

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

	struct *pp = (struct prime_pair*)malloc(sizeof(prime_pair));
	pp->p = p;
	pp->q = q;
	pp->phi_n = phi_n;

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


struct
certificate* create_certificate(char *name, int sz){

	/*R*/
	assert(sz < 7);
	int i;
	struct person* cp = (struct person*)malloc(sizeof(struct person));
	struct certificate* cf = (struct certificate*) malloc(sizeof(struct certificate));
	char *pname;
	char *cr;
	/*Name padding*/
	if (sz < 6) {
		for(i = 0; i < 6 - strlen(name); i++){
			strcat(pname,' ');
		}
		strcat(pname,name);
		assert(strlen(pname) == 6);
	}

	/*Bit representation of n*/
	struct key_pair* kp = generate_key_pair();
	bit_r *bn = bit_representation(kp->p * kp->q);
	bit_r *be = bit_representation(kp->e);
	strcat(cr,name);
	strcat(cr,bn);
	strcat(cr,be);

	/*Create a certificate*/
	cf->r = cr;
	char *current = NULL;
	/*Compute h(r)*/
	for(i = 1; i <= strlen(cf->r); i++){
		strcat(current,cf->r[i]);
		if(i % 8 == 0){
			cf->h ^= current;
			current = NULL;
		}
	}
	/*S*/
	/**Signing*/

	/*Should be 8 bits*/
	assert(strlen(cf->h) == 8);

	bit_r *h = (struct bit_r*) malloc(sizeof(struct bit_r));
	h->bits  = cf->h;
	h->sz    = strlen(cf->h);

	/*Store it as an int_r*/
	int_r hs = bits_to_int_r(h);
	cf->hs   = hs;



}




int main(){


}
