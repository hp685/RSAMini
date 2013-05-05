#ifndef _CERTIFICATE_H
#define _CERTIFICATE_H



struct person{

	char* name;
	int name_sz;
	struct keypair kp;
}

/**/
struct certificate{

	char* r;
	char* signature;
	int signature_sz;

}

#endif
