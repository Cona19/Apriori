#include "unit_test.h"
#include <stdio.h>

void trx_list_test(std::vector<trx_t> &trx_list){
	for (std::vector<trx_t>::iterator it = trx_list.begin(); it != trx_list.end(); it++){
		printf("{ ");
		for (trx_t::iterator it2 = it->begin(); it2 != it->end(); it2++){
			printf("%d ",*it2);
		}
		printf("}\n");
	}
}