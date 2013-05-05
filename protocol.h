#ifndef PROTOCOL_H
#define PROTOCOL_H
#endif


struct key_pair{

	int_r e, d, phi_n, p, q;
};


/*   n = (p * q)    */
struct prime_pair{

	int_r p, q;
	int_r phi_n;
};

struct key_pair* generate_key_pair();
struct prime_pair* generate_prime_pair();
