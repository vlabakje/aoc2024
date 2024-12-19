#include <bitset>
#include <cmath>
#include <limits>
#include <deque>
#include <iostream>
#include <fstream>
#include <vector>
#include <ranges>
#include <set>
#include <regex>


std::vector<int> machine(int64_t a, int64_t b, int64_t c, const std::vector<int>& instr) {
    std::vector<int> out;
    int ip = 0;
    auto combo = [&]() -> int64_t{
        if (instr[ip+1] > 0 and instr[ip+1] <= 3) return instr[ip+1];
        if (instr[ip+1] == 4) return a;
        if (instr[ip+1] == 5) return b;
        if (instr[ip+1] == 6) return c;
        if (instr[ip+1] == 7) std::runtime_error("invalid program");
        return -1;
    };
    while (ip >= 0 and ip < static_cast<int>(instr.size())) {
        switch (instr[ip]) {
            case 0: // adv (devision)
                a = a / std::pow(2, combo());
                break;
            case 1: // bxl
                b = b ^ instr[ip+1];
                break;
            case 2: // bst
                b = 7 & combo();
                break;
            case 3: // jnz
                if (a > 0) {
                    ip = instr[ip+1];
                    continue;
                }
                break;
            case 4: // bxc
                b = b ^ c;
                break;
            case 5: // out
                out.push_back(7 & combo());
                break;
            case 6: // bdv
                b = a / std::pow(2, combo());
                break;
            case 7: // cdv
                c = a / std::pow(2, combo());
                break;
        }
        ip += 2;
    }
    return out;
}

int64_t self_replicating_a(int64_t b, int64_t c, std::vector<int>& instr, int64_t a, int64_t i) {
    /** 
     * analyzing the program it turns out every 3 bits of a result in one number being output
     * the leftmost bits influence the digits further down the right so we start out
     * on the left, setting all 8 different 3 bit combinations.
     * if it yields the right number in that spot on the output, we recurse to find
     * valid outputs and the minimum number that generate it.
     */
    if (i == -1) return a;
    int64_t min = std::numeric_limits<int64_t>::max();
    int64_t x = a;
    for (int64_t j=0; j < 8; j++) {
        x = a | (j << (i*3));
        auto output = machine(x, b, c, instr);
        if (output.size() != instr.size()) continue;
        if (output[i] != instr[i]) continue;
        min = std::min(min, self_replicating_a(b, c, instr, x, i-1));
    }
    return min;
}

int64_t run_program(const std::string& filename) {
	std::ifstream file(filename);
	if (!file) throw std::runtime_error("No such file: " + filename);

    std::string contents((std::istreambuf_iterator<char>(file)),
                          std::istreambuf_iterator<char>());

    std::regex pattern(R"(\w+ A: (\d+)\n\w+ B: (\d+)\n\w+ C: (\d+)\n\n\w+: ([\d,]+))");
    std::smatch match;
    if (!std::regex_search(contents.cbegin(), contents.cend(), match, pattern))
            std::runtime_error("regex not parsed");
    if (match.size() != 5)
            std::runtime_error("could not find pattern!");
    std::vector<int> instr;
    for (auto &&  part: match[4].str() | std::views::split(',')) {
        instr.push_back(std::stoi(std::string(part.begin(), part.end())));
    }
    return self_replicating_a(
            std::stoi(match[2].str()), 
            std::stoi(match[3].str()), 
            instr, 0, static_cast<int>(instr.size()) -1);
}

int main() {
    std::cout << run_program("example2") << std::endl;
    std::cout << run_program("input") << std::endl;
    return 0;
}
