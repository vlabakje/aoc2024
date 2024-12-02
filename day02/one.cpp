#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>

void reportsFromFile(const std::string& filename, std::vector<int>& left, std::vector<int>& right) {
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
			right.push_back(second);
		} else {
			throw std::runtime_error("unsupported format: " + line);
		}
	}
	file.close();
}

bool safe(std::vector<int>& levels) {
	int c = levels[0];
	bool up = levels[1] - levels[0] > 0;
	for (size_t i = 1; i < levels.size(); ++i) {
		if ((up and levels[i] < c) or (not up and levels[i] > c)){
			return false;
		}
		if ((abs(levels[i] - c) > 3) or (levels[i] == c)) {
			return false;
		}
		c = levels[i];
	}
	return true;
}

int safe_count(const std::string& filename) {
	int count = 0;
	std::ifstream file(filename);
	if (!file) {
		throw std::runtime_error("No such file: " + filename);
	}

	std::string line;
	while (std::getline(file, line)) {
		std::istringstream stream(line);
		std::vector<int> levels;
		int n;

		while (stream >> n) {
			levels.push_back(n);
		}

		if (safe(levels)) {
			count += 1;
		}
	}
	return count;
}

int main() {
	std::cout << safe_count("example") << std::endl;
	std::cout << safe_count("input") << std::endl;
	return 0;
}
