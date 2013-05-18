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
	printf("%s\n",x->bits);
	int i;

	for(i = strlen(x->bits) ; i > 0; i--){
		y = (y * y) % c;
		if(x->bits[MAX_INT_LEN - i] == '1'){
			y = (a * y) % c;

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
		if(is_coprime == 1){
			/*Find the multiplicative inverse d*/
			struct ipair r = eea_gcd(i, pp->phi_n);
			printf("Mul Inv: %d %d %d %d\n",i, pp->phi_n, r.first, r.second);
			kp->e     = i;
			kp->d     = (pp->phi_n + r.first) % pp->phi_n;
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
	while(q == p){
		q = candidate_primes();
	}
	int_r phi_n = (p - 1) * (q - 1);

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
	int i;

	struct certificate* cf = (struct certificate*) malloc(sizeof(struct certificate));
	char *cr =(char*) malloc(sizeof(char) * 150);

	/*Bit representation of n*/

	struct bit_r *bn = bit_representation(p1->kp->p * p1->kp->q);
	struct bit_r *be = bit_representation(p1->kp->e);
#ifdef TRACE
	printf("%s\n",p1->name);
	printf("%s\n",bn->bits);
	printf("%s\n",be->bits);
#endif
	strcat(cr,(const char*)p1->name);
	strcat(cr,(const char*)bn->bits);
	strcat(cr,(const char*)be->bits);

	/*Create a certificate*/
	cf->r =(char*) malloc(sizeof(char) * strlen(cr));

	strcpy(cf->r, (const char*)cr);

	printf("161: R(In Bits) = %s\n",cf->r);

	char *current = (char*) malloc(sizeof(char) * 10);


	/*Compute h(r)*/
	for(i = 0; i < strlen(cr); i++){

		char a[2];
		a[0]= cr[i];
		a[1] = '\0';
		strcat(current, (const char*)a);

		if((i + 1) % 8  == 0){
			struct bit_r *b = (struct bit_r*)malloc(sizeof(struct bit_r));
			b->bits = (char*) malloc(sizeof(char*) * strlen(current));
			strcpy(b->bits,current);
			cf->hs ^= bits_to_int_r(b);
			memset(current,'\0',sizeof(current));
			free(b->bits);
			free(b);

		}
	}

	/*S*/
	/**Signing*/

	/*Should be 8 bits*/

	printf("%d\n",p2->kp->e);
	cf->signature = fast_exponentiation(cf->hs, p2->kp->e, p2->kp->p * p2->kp->q);
	printf("164: Signature s = %d\n",cf->signature);
}

int_r compute_hash_decrypt(struct person *p, int_r u){

	struct bit_r *cr = bit_representation(u);
	int_r h = 0, i;
	char *current = (char*) malloc(sizeof(char) * 10);

	/*Compute Hash*/
	for(i = 0; i < strlen(cr->bits); i++){

		char a[2];
		a[0]= cr->bits[i];
		a[1] = '\0';
		strcat(current, (const char*)a);

		if((i + 1) % 8  == 0){

			struct bit_r *b = (struct bit_r*)malloc(sizeof(struct bit_r));
			b->bits = (char*) malloc(sizeof(char*) * strlen(current));
			strcpy(b->bits,current);
			printf("%s\n",b->bits);
			h ^= bits_to_int_r(b);
			memset(current,'\0',sizeof(current));
			free(b->bits);
			free(b);

		}
	}

	free(cr->bits);
	free(cr);
	printf("163: h: %d\n",h);
	/*Decrypt using private Key*/
	int_r d = fast_exponentiation(h, p->kp->e, p->kp->p * p->kp->q);
	printf("191: V = %d\n",d);
	return d;
}


int main(){

	srand(getpid());
	printf("_____________________BEGIN TRACE_________________\n");
	int i;

	/* for(i = 0; i < 20; i++){ */
	/* 	int_r prime = candidate_primes(); */
	/* 	printf("%d\n",prime); */
	/* } */


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
	create_certificate(p1, p2);
	int_r u;
	/*Get the bit representation of Alice's n*/
	struct bit_r *bu = bit_representation(p1->kp->p * p1->kp->q);

	char *cu  = malloc(sizeof(char) * MAX_INT_LEN);

	for(i = 0; i < strlen(bu->bits); i++ ){
		if(bu->bits[i] == '1')
			break;
		cu[i] = '0';
	}

	int j;
	cu[i] = '1';
#ifdef TRACE
	printf("181: K= %d\n", MAX_INT_LEN - i - 1);
#endif
	for(j = i+1; j < strlen(bu->bits); j++){
		int rb = generateRandomBits() % 2;
		cu[j] = rb + '0';

	}

	struct bit_r *cbu = (struct bit_r *) malloc(sizeof(struct bit_r));
	cbu->bits = (char*) malloc(sizeof(char) * MAX_INT_LEN);
	strcpy(cbu->bits, cu);
	u = bits_to_int_r(cbu);
#ifdef TRACE
	printf("182: U = %d\n",u);
	printf("181: %s\n",cbu->bits);
#endif

	/*Send u to Alice so that she can compute h(u)*/

	int_r d = compute_hash_decrypt(p1,u);
	/*Bob encrypts with Alice's public key*/

	int hu = fast_exponentiation(d,p1->kp->d, p1->kp->p * p1->kp->q);
	printf("191: hu: %d\n",hu);
	struct bit_r * t = bit_representation(p1->kp->p * p1->kp->q);
	printf("N(In bits): %s\n", t->bits);
	printf("N, p ,q: %d,%d,%d\n",p1->kp->p * p1->kp->q,p1->kp->p , p1->kp->q);
	printf("e,d(Alice): %d,%d\n", p1->kp->e, p1->kp->d);
	//TEST	printf("%d\n",fast_exponentiation(5,3,13));
	printf("\n");
	printf("__________________END OF TRACE_________________\n");

	free(bu->bits);
	free(bu);
	free(cbu->bits);
	free(cbu);
	free(t->bits);
	free(t);
	free(cu);
	free(p1->kp);
	free(p1);
	free(p2->kp);
	free(p2);

	return 0;
}
