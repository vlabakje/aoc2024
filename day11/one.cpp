#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>

int digits(int64_t num) {
    if (num == 0) return 1;
    num = std::abs(num);
    return static_cast<int>(std::log10(num)) + 1;
}

std::pair<int64_t, int64_t> split(uint64_t num) {
    int d = static_cast<int>(std::pow(10, digits(num)/2));
    return {num / d, num % d};
}

int stone_length(int64_t stone, int steps) {
    if (stone < 0) {
        throw std::runtime_error("overflow");
    }
    //std::string indent(25-steps, ' ');
    //std::cout << indent << "step: " << steps << " stone: " << stone << std::endl;
    if (steps == 0) {
        return 1;
    }
    if (stone == 0) {
        return stone_length(1, steps-1);
    }
    if (digits(stone) % 2 == 0) {
        // split based on string
        auto [a, b] = split(stone);
        //std::cout << " split a=" << a << " b=" << b << " digits=" << digits(stone) << std::endl;
        return stone_length(a, steps - 1) + stone_length(b, steps - 1);
    }
    return stone_length(stone * 2024, steps-1);
}

int moving_stones(const std::string& filename, int blinks) {
	std::ifstream file(filename);
	if (!file) {
		throw std::runtime_error("No such file: " + filename);
	}

	std::vector<int64_t> stones;
	std::string line;
	while (std::getline(file, line)) {
        std::stringstream ss(line);
        int64_t value;
        while (ss >> value) {
            stones.push_back(value);
        }
	}

    int size = 0;
    for (auto& stone: stones) {
        size += stone_length(stone, blinks);
    }
    return size;
}

int main() {
	std::cout << moving_stones("example", 25) << std::endl;
	std::cout << moving_stones("input", 25) << std::endl;
	return 0;
}
