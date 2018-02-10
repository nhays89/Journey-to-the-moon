#include <cstdio>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
using namespace std;
/*
A group is a related set of astronauts that are from
the same country of origin.
An astronaut may have a direct parent group, and then
later be merged with another known group that is from the
same country of origin with a new root group. It will retain
the original parent group.

For instance, example input:
53
01
23
12

The astronauts, 0 and 1, are added to the map under the same group, A.

 A
/ \
0  1

Astronauts 2 and 3, are added the map under the same group, B.

 A
/ \
0  1
 B
/ \
2  3

Astronauts 1 and 2 already exist, but each belongs to a different 'root' group, namely A and B.
So, a new group, C, is created, and both A and B point to the new root group C.

	C
  /   \
 A     B
/ \   / \
0  1 2   3

*/
typedef struct group {
	long size;
	struct group *parent;
} Group;


/*
Calculates the number of unique pairwise combinations of astronauts.

@total_astros - the total number of astronauts for the sample case.
@num_of_known_astros - the total number of astronauts who belong to a group.
@root_group_set - set of root astronaut groups
@return - an integer denoting the total number of unique pairwise combinations of astronauts.

*/

long int get_combinations(int total_astros, int num_of_known_astros, unordered_set<Group *> & root_group_set) {
	int unknown_astro_size = total_astros - num_of_known_astros; // the number of astronauts where the group is unknown
	int num_root_groups = root_group_set.size();
	vector<int> root_group_size(num_root_groups);
	int i = 0;
	for (auto itr = root_group_set.begin(); itr != root_group_set.end(); ++itr) {
		root_group_size[i++] = ((*itr)->size);
	}

	long int num_of_combinations = 0;
	for (i = 0; i < num_root_groups; i++) {
		for (int j = i + 1; j < num_root_groups; j++) {
			num_of_combinations += root_group_size[i] * root_group_size[j];
		}
		num_of_combinations += root_group_size[i] * unknown_astro_size;
	}
	num_of_combinations += (long int)unknown_astro_size * (unknown_astro_size - 1) / 2;
	return num_of_combinations;
}


/*
@total_astros - the total number of astronauts for the sample case.
@pairs - input list of astronaut pairs.
@return - an integer denoting the total number of pairwise combinations of astronauts belonging to different countries.

Traverses through a list of pairs of astronauts known to belong to the same country. Using
transitive reduction via common root groups, groups of astronauts are formed and merged
building larger groups to represent the growing number of astronauts for each country.
Each astronaut can belong to many parent groups, but only one of which is the root parent group.
The root of which is the country origin for that astronaut.

An unordered_map stores data about each astronauts group(s), and an unordered_set stores data
on all root groups. Once all groups are created, the set will be iterated over to access the count
of each root group.A sum of all pairwise combinations of counts is then calculated for known groups
followed by a sum of all pairwise combinations of unknown groups.
These totals are added and returned.
*/
long int comp(int total_astros, vector<vector<int>> & pairs) {

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

	return get_combinations(total_astros, num_of_known_astros, root_group_set);
}

/*

Example input:
53 
01
23
12

where:
5 is total number of astronauts
3 is the number of pairs of astronauts listed below on seperate lines.

*/

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