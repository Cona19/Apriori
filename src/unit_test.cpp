#include "unit_test.h"
#include <stdio.h>

void trx_list_test(trx_list_t trx_list){
    printf("trx_list_test\n");
	for (trx_list_t::iterator it = trx_list.begin(); it != trx_list.end(); it++){
		printf("{ ");
		for (trx_t::iterator it2 = it->begin(); it2 != it->end(); it2++){
			printf("%d ",*it2);
		}
		printf("}\n");
	}
}

void cand_map_test(cand_map_t cand_map){
    printf("cand_map_test\n");
    for (cand_map_t::iterator it = cand_map.begin(); it != cand_map.end(); it++){
        printf("{ ");
        for (trx_t::iterator it2 = it->first.begin(); it2 != it->first.end(); it2++){
            printf("%d ", *it2);
        }
        printf("} : ");
        printf("%d\n", it->second);
    }
}
/*
void itemset_map_test(itemset_map_t itemset_map){
    printf("itemset_map_test\n");
    for (itemset_map_t::iterator it = itemset_map.begin(); it != itemset_map.end(); it++){
        printf("{ ");
        for (itemset_t::iterator it2 = it->first.begin(); it2 != it->first.end(); it2++){
            printf("%d ", *it2);
        }
        printf("} : %d\n", it->second);
    }
}
*/
