#ifndef _CERTIFICATE_H
#define _CERTIFICATE_H

#define NUM_BYTES 14

struct key_pair{

	int_r e;
	int_r d;
	int_r phi_n;
	int_r p, q;
};


/*   n = (p * q)    */
struct prime_pair{

	int_r p, q;
	int_r phi_n;
};


/**/
struct certificate{

	char* r;
	char* h;
	int_r hs; /*Signature stored as an int_r*/
	int_r signature; /*decrypted with Trent's private key*/
};


struct person{

	char* name;
	struct key_pair* kp;
	struct certificate* cf;
};


struct key_pair* generate_key_pair();
struct prime_pair* generate_prime_pair();
int_r fast_exponentiation(int_r, int_r, int_r);
void create_certificate(struct person* , struct person* );
int_r compute_hash_decrypt(struct person*, int_r);


#endif
