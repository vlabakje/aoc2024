#include <cmath>
#include <deque>
#include <iostream>
#include <fstream>
#include <vector>
#include <ranges>
#include <set>
#include <regex>

uint64_t mix(uint64_t a, uint64_t b) {
    return a ^ b;
}

uint64_t prune(uint64_t n) {
    return n % 16777216;
}

uint64_t secret(uint64_t n) {
    n = prune(mix(n, n * 64));
    n = prune(mix(n, n / 32));
    n = prune(mix(n, n * 2048));
    return n;
}

uint64_t run_program(const std::string& filename) {
	std::ifstream file(filename);
	if (!file) throw std::runtime_error("No such file: " + filename);

    uint64_t result = 0;
    std::string line;
    while (std::getline(file, line)) {
        int64_t num = std::stoi(line);
        for (int i=0; i<2000; i++) num = secret(num);
        //std::cout << num << std::endl;
        result += num;
    }
    return result;
}

int main() {
    std::cout << run_program("example") << std::endl;
    std::cout << run_program("input") << std::endl;
    return 0;
}
