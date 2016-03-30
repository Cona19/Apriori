#include <stdlib.h>
#include <vector>
#include <set>
#include <map>

typedef int item_t;
typedef std::set<item_t> trx_t;
typedef std::vector<trx_t> trx_list_t;
typedef std::map<trx_t, int> cand_map_t;
