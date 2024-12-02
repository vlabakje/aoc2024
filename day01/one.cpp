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

int total_distance(const std::string& filename){
	int sum = 0;
	std::vector<int> left, right;
	try {
		reportsFromFile(filename, left, right);
		std::sort(left.begin(), left.end());
		std::sort(right.begin(), right.end());
		// loop over left+right together
		for (size_t i = 0; i < left.size(); ++i) {
			sum += std::abs(left[i] - right[i]);
		}
	} catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}
	return sum;
}

int main() {
	std::cout << total_distance("example") << std::endl;
	std::cout << total_distance("input") << std::endl;
	return 0;
}
