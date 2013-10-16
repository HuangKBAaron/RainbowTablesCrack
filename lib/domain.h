#define MAY_DEFAULT 0
#define MIN_DEFAULT 1
#define NUMBER_DEFAULT 0


#define DOM_MAX 100		/* Maximum domain cardinality. */




struct domain {
	char elements[DOM_MAX];
	int length;
};




void set_key_domain(struct domain *k_domain, char *tag);



