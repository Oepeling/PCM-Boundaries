#include <iostream>
#include <fstream>
#include <string>
#include "graph-builder.cpp"

using namespace std;

void Read(const string input_file, vector<Param>& from_in) {
	ifstream in(input_file);

	string s;
	while (getline(in, s)) {
		Param param;

		for (int i = 0, j = 1; i < s.size(); j++, i++) {
			string el = "";
			while (i < s.size() && s[i] != ';') {
				el += s[i];
				i++;
			}
			if (j == 2) {
				param.id = el;
			} else if (j == 3) {
				param.desc = el;
			} else if (j == 4) {
				param.deps = el;
			} else if (el == "0") {
				param.count_0++;
			} else if (el == "-") {
				param.count_minus++;
			} else if (el == "+") {
				param.count_plus++;
			}
		}

		from_in.push_back(param);
	}

	in.close();
}

int main() {
	vector<Param> from_in;
	Read("input.csv", from_in);

	Dag dag;
	for (auto param : from_in) {
		dag.addParam(param);
	}

	dag.buildDag();
	dag.countSubtrees();
	dag.printParams();

	return 0;
}