#ifndef CERTIFICATE_H
#define CERTIFICATE_H
#endif


struct person{

	char* name;
	int name_sz;

}

/**/
struct certificate{

	struct person p;
	char* signature;
	int signature_sz;

}
