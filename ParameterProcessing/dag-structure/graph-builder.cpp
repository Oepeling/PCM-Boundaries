#include <vector>
#include <map>
#include <string>
#include <cassert>
#include <iostream>

using namespace std;

bool isUpperLetter(char c) {
	return (c >= 'A' && c <= 'Z');
}

struct Param {
	string id;
	string desc;
	string deps;

	int total_deps = 0;

	int count_0 = 0;
	int count_minus = 0;
	int count_plus = 0;
};

class Dag {
	vector<Param> nodes_;
	vector<vector<int>> edges_[2];
	map<string, int> id_to_index_;
	int size_ = 0;

	int dfs(int node, vector<bool>& viz) {
		viz[node] = true;
		int result = 1;

		for (auto child : edges_[0][node]) {
			if (!viz[child]) {
				result += dfs(child, viz);
			}
		}

		return result;
	}

	vector<int> getDeps(const Param& param) {
		vector<int> result;
		string deps = param.deps;

		for (int i = 0; i < deps.size(); i++) {
			if (!isUpperLetter(deps[i])) { continue; }

			string dep = deps.substr(i, 3);
			i += 3;

			assert(id_to_index_.find(dep) != id_to_index_.end());
			result.push_back(id_to_index_[dep]);
		}

		return move(result);
	}

public:
	Dag() {}	

	void addParam(const Param& param) {
		id_to_index_[param.id] = size_;
		nodes_.push_back(param);
		edges_[0].push_back(vector<int>());
		edges_[1].push_back(vector<int>());
		size_++;

		assert(size_ == nodes_.size());
	}

	void buildDag() {
		for (int i = 0; i < size_; i++) {
			vector<int> deps = getDeps(nodes_[i]);
			for (auto dep : deps) {
				edges_[0][dep].push_back(i);
				edges_[1][i].push_back(dep);
			}
		}
	}

	void countSubtrees() {
		for (int i = 0; i < size_; i++) {
			vector<bool> viz(size_);
			nodes_[i].total_deps = dfs(i, viz);
		}
	}

	void printDeps() {
		cout << size_ << "\n";
		for (int i = 0; i < size_; i++) {
			cout << i << " " << nodes_[i].id << " : ";
			for (auto dep : edges_[1][i]) {
				cout << nodes_[dep].id << " ";
			}
			cout << "\n";
		}
	}
	
	void printRevDeps() {
		for (int i = 0; i < size_; i++) {
			cout << i << " " << nodes_[i].id << " " << nodes_[i].total_deps << " : ";
			for (auto dep : edges_[0][i]) {
				cout << nodes_[dep].id << " ";
			}
			cout << "\n";
		}	
	}

	void printParams() {
		for (int i = 0; i < size_; i++) {
			cout << i << " " << nodes_[i].id << " "
				<< nodes_[i].total_deps << " ";
			cout << nodes_[i].count_0 << " "
				<< nodes_[i].count_plus << " "
				<< nodes_[i].count_minus << "\n";
			// cout << "    "
			// 	<< "0: " << nodes_[i].count_0
			// 	<< " +: " << nodes_[i].count_plus
			// 	<< " -: " << nodes_[i].count_minus << "\n";
		}
	}
};