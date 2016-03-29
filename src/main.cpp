#include <stdio.h>
#include <string.h>
#include "types.h"
#include "config.h"

#ifdef UNIT_TEST
#include "unit_test.h"
#endif

void init_trx_list(char *in_file, std::vector<trx_t> *trx_list){
	char buf[BUF_SIZE];
	FILE *in = fopen(in_file, "r");
	while(fgets(buf, BUF_SIZE, in) != NULL){
		char *token = strtok(buf, "\t");
		trx_t new_node;
		new_node.clear();
		while(token != NULL){
			new_node.insert(atoi(token));
			token = strtok(buf, "\t");
		}
		trx_list->insert(trx_list->end(), new_node.begin(), new_node.end());
	}
	fclose(in);
}

int main(int argc, char *argv[]){
	if (argc != 4){
		printf("Usage : %s minimum_support input_file_path output_file_path\n");
		return 0;
	}
	std::vector<trx_t> trx_list;
	double minimum_support;
	char *in_file = argv[2];
	char *out_file = argv[3];

	
	init_trx_list(in_file, &trx_list);

	trx_list_test(trx_list);

	return 0;
}