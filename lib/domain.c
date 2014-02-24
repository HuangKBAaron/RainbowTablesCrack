#include <string.h>
#include "domain.h"



void 
set_key_domain(struct domain *k_domain, char *tag){
	char i;
	char *pch;

	k_domain->length = 0;

	pch = strchr(tag,'a');
	if(pch != NULL){
		for(i = 'a'; i <= 'z' ; i++){
			k_domain->elements[k_domain->length] = i;
			k_domain->length++;
		}
	}
	pch = strchr(tag,'A');
	if(pch != NULL){
		for(i = 'A'; i <= 'Z' ; i++){
			k_domain->elements[k_domain->length] = i;
			k_domain->length++;
		}
	}
	pch = strchr(tag,'0');
	if(pch != NULL){
		for(i = '0'; i <= '9' ; i++){
			k_domain->elements[k_domain->length]= i;
			k_domain->length++;
		}
	}
	pch = strchr(tag,'*');
	if(pch != NULL){
		k_domain->elements[k_domain->length]= '.';
		k_domain->length++;

		k_domain->elements[k_domain->length]= '_';
		k_domain->length++;
	}
}





