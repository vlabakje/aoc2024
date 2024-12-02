#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <unordered_map>

void reportsFromFile(const std::string& filename, std::vector<int>& left, std::unordered_map<int, int>& right) {
	std::ifstream file(filename);
	if (!file) {
		throw std::runtime_error("No such file: " + filename);
	}

	std::string line;
	while (std::getline(file, line)) {
		std::istringstream stream(line);
		int first, second;
		if (stream >> first >> second) {
			left.push_back(first);
			if (right.find(second) == right.end()) {
				right[second] = 0;
			}
			right[second] += 1;
		} else {
			throw std::runtime_error("unsupported format: " + line);
		}
	}
	file.close();
}

int similarity_score(const std::string& filename){
	int sum = 0;
	std::vector<int> left;
	std::unordered_map<int, int> right;
	try {
		reportsFromFile(filename, left, right);
		for (size_t i = 0; i < left.size(); ++i) {
			if (right.find(left[i]) != right.end()) {
				sum += left[i] * right[left[i]];
			}
		}
	} catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}
	return sum;
}

int main() {
	std::cout << similarity_score("example") << std::endl;
	std::cout << similarity_score("input") << std::endl;
	return 0;
}
