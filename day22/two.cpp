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

std::vector<std::array<int, 4>> combinations() {
    std::vector<std::array<int, 4>> out; // pow(19, 4)
    for (int s1 = -9; s1 < 10; s1++)
        for (int s2 = -9; s2 < 10; s2++)
            for (int s3 = -9; s3 < 10; s3++)
                for (int s4 = -9; s4 < 10; s4++)
                    out.push_back({s1, s2, s3, s4});
    return out;
}

uint64_t buyer_to_price(const std::vector<int>& price, const std::vector<int>& delta, const std::array<int, 4>& combi) {
    for (size_t i = 1; i < price.size() - 5; i++) {
        if (delta[i] == combi[0] and delta[i+1] == combi[1] and
            delta[i+2] == combi[2] and delta[i+3] == combi[3])
                return price[i+3];
    }
    return 0;
}

uint64_t run_program(const std::string& filename) {
	std::ifstream file(filename);
	if (!file) throw std::runtime_error("No such file: " + filename);

    std::string line;
    std::vector<std::vector<int>> prices;
    std::vector<std::vector<int>> deltas;
    while (std::getline(file, line)) {
        int64_t num = std::stoi(line);
        std::vector<int> price;
        std::vector<int> delta;
        delta.push_back(0);
        for (int i=0; i<2000; i++) {
            num = secret(num);
            price.push_back(num%10);
            if (i != 0) delta.push_back(price.back() - price.end()[-2]);
        }
        prices.push_back(price);
        deltas.push_back(delta);
    }
    // for every set of combinations, record the max score
    uint64_t max_score = 0;
    for (auto combi: combinations()) {
        uint64_t score = 0;
        for (size_t i=0; i<prices.size(); i++) {
            score += buyer_to_price(prices[i], deltas[i], combi);
        }
        if (score > max_score) {
            std::cout << "score=" << score << " c=" << combi[0] <<
                " " << combi[1] << " " << combi[2] << " " << combi[3] << std::endl;
        }
        max_score = std::max(max_score, score);
    }
    return max_score;
}

int main() {
    std::cout << run_program("example2") << std::endl;
    std::cout << run_program("input") << std::endl;
    return 0;
}
