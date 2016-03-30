#include <stdio.h>
#include <string.h>
#include <math.h>

#include "types.h"
#include "config.h"

#ifdef UNIT_TEST
#include "unit_test.h"
#endif

/*
Function Name : init_trx_list

*/
void init_trx_list(const char *in_file, trx_list_t &trx_list, cand_map_t &cand_map){
	char buf[BUF_SIZE];
	FILE *in = fopen(in_file, "r");

    trx_list.clear();
    cand_map.clear();

	while(fgets(buf, BUF_SIZE, in) != NULL){
		char *token = strtok(buf, "\t");
		trx_t new_node;

		while(token != NULL){
            int val = atoi(token);

			new_node.insert(val);
            itemset_t new_cand;

            new_cand.insert(val);
            cand_map[new_cand]++;
			token = strtok(NULL, "\t");
		}
		trx_list.push_back(new_node);
	}
	fclose(in);
}

cand_map_t join_freq_itemset(cand_map_t cand_map){
    cand_map_t new_cand_map;
    for (cand_map_t::iterator it = cand_map.begin(); it != cand_map.end(); it++){
        for (cand_map_t::iterator it2 = it; it2 != cand_map.end(); it2++){
            if (it == it2) continue;

            itemset_t::iterator i = it->first.begin();
            itemset_t::iterator i2 = it2->first.begin();
            itemset_t new_cand;

            while(i != it->first.end() && i2 != it2->first.end() && *i == *i2){
                new_cand.insert(*i);
                i++;
                i2++;
            }

            if (i == it->first.end() || i2 == it2->first.end()) continue;

            new_cand.insert(*i);
            new_cand.insert(*i2);

            i++;
            i2++;
            if (i == it->first.end() && i2 == it2->first.end()){
                new_cand_map[new_cand] = 0;
            }
        }
    }
    return new_cand_map;
}

cand_map_t cut_infreq_itemset(const int cnt_support, cand_map_t cand_map){
    for (cand_map_t::iterator it = cand_map.begin(); it != cand_map.end();){
        cand_map_t::iterator tmp = it;
        it++;
        if (tmp->second < cnt_support){
            cand_map.erase(tmp);
        }
    }
    return cand_map;
}

void get_cnt_cand(trx_list_t trx_list, cand_map_t &cand_map){
    for (cand_map_t::iterator it = cand_map.begin(); it != cand_map.end(); it++){
        for (trx_list_t::iterator it2 = trx_list.begin(); it2 != trx_list.end(); it2++){
            bool is_all_exist = true;
            for (itemset_t::iterator it3 = it->first.begin(); it3 != it->first.end(); it3++){
                if (it2->find(*it3) == it2->end()){//this itemset is not exist in this transaction
                    is_all_exist = false;
                    break;
                }
            }
            if (is_all_exist){//all item in itemset is exist in this transaction
                cand_map[it->first]++;
            }
        }
    }
}

cand_map_t make_candidate(const int cnt_support, const trx_list_t trx_list, cand_map_t &prev_cand_map){
    cand_map_t all_freq_itemset_map;

    prev_cand_map = cut_infreq_itemset(cnt_support, prev_cand_map);

#ifdef UNIT_TEST
    cand_map_test(prev_cand_map);
    int i = 1;
#endif

    while(!prev_cand_map.empty()){
        all_freq_itemset_map.insert(prev_cand_map.begin(), prev_cand_map.end());

#ifdef UNIT_TEST
        printf("%d번째 iteration\n", i++);
#endif
        cand_map_t next_cand_map = join_freq_itemset(prev_cand_map);
#ifdef UNIT_TEST
    cand_map_test(next_cand_map);
#endif
        get_cnt_cand(trx_list, next_cand_map);

#ifdef UNIT_TEST
    cand_map_test(next_cand_map);
#endif
        
        prev_cand_map = cut_infreq_itemset(cnt_support, next_cand_map);

#ifdef UNIT_TEST
    cand_map_test(prev_cand_map);
#endif
    }
    return all_freq_itemset_map;
}

void print_rules_from_itemset(FILE *out, const int trx_size, cand_map_t all_freq_itemset_map, 
                                itemset_t::iterator cur_item, itemset_t::iterator end, itemset_t cur_itemset,
                                itemset_t t, itemset_t s){

    if (cur_item == end){
        if (t.size() == 0 || s.size() == 0) return;
        itemset_t::iterator it = t.begin();
        fprintf(out, "{%d", *it);
        for (++it; it != t.end(); it++){
            fprintf(out, ",%d", *it);
        }
        fprintf(out,"}\t");

        it = s.begin();
        fprintf(out, "{%d", *it);
        for (++it; it != s.end(); it++){
            fprintf(out, ",%d", *it);
        }
        fprintf(out, "}\t%.2lf\t%.2lf\n", 100.0 * all_freq_itemset_map.find(cur_itemset)->second / trx_size, 100.0 * all_freq_itemset_map.find(cur_itemset)->second / all_freq_itemset_map.find(t)->second);

        return;
    }
    itemset_t next_t = t;
    itemset_t next_s = s;

    next_t.insert(*cur_item);
    next_s.insert(*cur_item);
    cur_item++;

    print_rules_from_itemset(out, trx_size, all_freq_itemset_map, cur_item, end, cur_itemset, next_t, s);
    print_rules_from_itemset(out, trx_size, all_freq_itemset_map, cur_item, end, cur_itemset, t, next_s);
}

void print_rule(const char *out_file, const int trx_size, cand_map_t all_freq_itemset_map){
    FILE *out=fopen(out_file, "w");
    for (cand_map_t::iterator it = all_freq_itemset_map.begin(); it != all_freq_itemset_map.end(); it++){
        if (it->first.size() <= 1) continue; 

        print_rules_from_itemset(out, trx_size, all_freq_itemset_map, it->first.begin(), it->first.end(), it->first,
                                 itemset_t(), itemset_t());
    }
    fclose(out);
}

int main(int argc, char *argv[]){
	if (argc != 4){
		printf("Usage : %s minimum_support input_file_path output_file_path\n", argv[0]);
		return 0;
	}

	trx_list_t trx_list;
    cand_map_t first_cand_map;
    cand_map_t all_freq_itemset_map;
	double minimum_support = atof(argv[1]);
	char *in_file = argv[2];
	char *out_file = argv[3];
    
	init_trx_list(in_file, trx_list, first_cand_map);

#ifdef UNIT_TEST
	trx_list_test(trx_list);
    cand_map_test(first_cand_map);
#endif

    all_freq_itemset_map = make_candidate(ceil(trx_list.size() * minimum_support / 100.0), trx_list, first_cand_map);

#ifdef UNIT_TEST
    cand_map_test(all_freq_itemset_map);
#endif

    print_rule(out_file, trx_list.size(), all_freq_itemset_map);

	return 0;
}
