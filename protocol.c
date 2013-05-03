#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<assert.h>
#include"rsa.h"

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


int main(){


}
