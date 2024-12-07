#include <algorithm>
#include <cstdint>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>

uint64_t concat(uint64_t a, uint64_t b) {
    uint64_t digits = (b == 0) ? 1 : static_cast<uint64_t>(std::log10(b)) + 1;
    uint64_t shift = static_cast<uint64_t>(std::pow(10, digits));
    if (a > UINT64_MAX / shift) {
        return UINT64_MAX;
    }
    uint64_t concatenated = a * shift + b;
    if (concatenated < a or concatenated < b) {
        return UINT64_MAX;
    }
    return concatenated;
}

bool can_solve(uint64_t result, std::vector<uint64_t>& values, uint64_t cum, int index) {
    //std::cout << result << " " << index << " " << cum << std::endl;
    if (index == values.size()) {
        return result == cum;
    }
    // try to solve in three ways
    if (result >= cum * values[index]) {
        if (can_solve(result, values, cum * values[index], index+1)) {
            return true;
        }
    }
    if (result >= cum + values[index]) {
        if (can_solve(result, values, cum + values[index], index+1)) {
            return true;
        }
    }
    if (result >= concat(cum, values[index])) {
        if (can_solve(result, values, concat(cum, values[index]), index+1)) {
            return true;
        }
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

    if (can_solve(result, values, values[0], 1)) {
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
        std::cout << "line=" << line << std::endl;
        total += single_result(line);
        std::cout << "total=" << total << std::endl;
	}
	return total;
}

int main() {
	std::cout << total_result("example") << std::endl;
	std::cout << total_result("input") << std::endl;
	return 0;
}
