#ifndef _CERTIFICATE_H
#define _CERTIFICATE_H


struct key_pair{

	int_r e, d, phi_n, p, q;
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


#endif
