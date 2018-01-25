#include <cstdio>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
using namespace std;

class Group {
public:
	int size;
	Group *parent;
};

const int s = 1;

int comp(int total_astros, vector<vector<int>> pairs) {

	unordered_map<int, Group *> map; // key: astronaut #, value: Group object
	unordered_set<Group *> root_group_set; // set of all groups which have no parent group
	int num_of_known_astros = 0;
	for (int i = 0; i < pairs.size(); i++) {
		vector<int> astro(pairs[i]);

		int map_size = map.size();
		bool is_new_astro_1 = false;
		bool is_new_astro_2 = false;
		int astro_1 = astro[0], astro_2 = astro[1];
		Group *astro_1_group = map[astro_1]; //grows by 1 and adds key if key is not in container
		if (map_size != map.size()) {
			is_new_astro_1 = true;
		}
		map_size = map.size();
		Group *astro_2_group = map[astro_2];

		if (map_size != map.size()) {
			is_new_astro_2 = true;
		}
		if (is_new_astro_1 && is_new_astro_2) { //if both new astronauts
			Group *g = new Group();
			g->parent = nullptr;
			g->size = 2;
			map[astro_1] = g;
			map[astro_2] = g;

			root_group_set.insert(g);
			num_of_known_astros += 2;
		}
		else if (is_new_astro_1) {
			
			map[astro_1] = astro_2_group;
			astro_2_group->size++;
			astro_2_group = astro_2_group->parent;
			//bubble up
			while (astro_2_group) {
				astro_2_group->size++;
				astro_2_group = astro_2_group->parent;
			}
			num_of_known_astros++;
		}
		else if (is_new_astro_2) {
			map[astro_2] = astro_1_group;
			astro_1_group->size++;
			astro_1_group = astro_1_group->parent;
			//bubble up
			while (astro_1_group) {
				astro_1_group->size++;
				astro_1_group = astro_1_group->parent;
			}
			num_of_known_astros++;
		}
		else {
			while (astro_1_group->parent) {
				astro_1_group = astro_1_group->parent;
			}
			while (astro_2_group->parent) {
				astro_2_group = astro_2_group->parent;
			}
			if (astro_1_group != astro_2_group) {
				//merge
				Group *g = new Group();
				g->parent = nullptr;
				g->size = astro_1_group->size + astro_2_group->size;
				root_group_set.erase(astro_1_group); //no longer root
				root_group_set.erase(astro_2_group); //no longer root
				root_group_set.insert(g);// a new root
				astro_2_group->parent = g;
				astro_1_group->parent = g;
			} //else if they are equal..ignore.
		}
	}
	
	int unknown_astro_size = total_astros - num_of_known_astros; // the number of astronauts where the group is unknown
	int num_root_groups = root_group_set.size();
	vector<int> root_group_size(num_root_groups);
	int i = 0;
	for (auto itr = root_group_set.begin(); itr != root_group_set.end(); ++itr) {
		root_group_size[i++] = ((*itr)->size);
	}
	
	int num_of_combinations = 0;
	for (i = 0; i < num_root_groups; i++) {
		for (int j = i + 1; j < num_root_groups; j++) {
			num_of_combinations += root_group_size[i] * root_group_size[j];
		}
		num_of_combinations += root_group_size[i] * unknown_astro_size;
	}
	num_of_combinations += unknown_astro_size * (unknown_astro_size - 1) / 2;


	return num_of_combinations;
}

int main() {
	int n, p;
	cin >> n;
	cin >> p;
	vector<vector<int>> v;
	int x, y;
	for (int i = 0; i < p; i++) {
		x = 0; y = 0;
		cin >> x;
		cin >> y;
		v.push_back({x, y});
	}
	cout << comp(n,  v);


	return 0;
}