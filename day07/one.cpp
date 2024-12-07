#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>

bool can_solve(uint64_t result, std::vector<uint64_t>& values) {
    // std::cout << result << " " << values.size() << std::endl;
    if (values.size() == 1) {
        return result == values[0];
    }
    
    std::vector<uint64_t> oneless(values.begin(), values.end() - 1);
    if (result % values.back() == 0) {
        if (can_solve(result / values.back(), oneless)){
            return true;
        }
    }
    if (result > values.back()) {
        return can_solve(result - values.back(), oneless);
    }
    return false;
}

uint64_t single_result(const std::string& line) {
    char colon;
    uint64_t result;
    std::vector<uint64_t> values;

    std::stringstream ss(line);
    ss >> result >> colon;

    int value;
    while (ss >> value) {
        values.push_back(value);
    }

    if (can_solve(result, values)) {
        return result;
    }
    return 0;
}

uint64_t total_result(const std::string& filename) {
	uint64_t total = 0;
	std::ifstream file(filename);
	if (!file) {
		throw std::runtime_error("No such file: " + filename);
	}

	std::string line;
	while (std::getline(file, line)) {
        // std::cout << "line=" << line << std::endl;
        total += single_result(line);
        // std::cout << "total=" << total << std::endl;
	}
	return total;
}

int main() {
	std::cout << total_result("example") << std::endl;
	std::cout << total_result("input") << std::endl;
	return 0;
}
