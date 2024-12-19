#include <cmath>
#include <deque>
#include <iostream>
#include <fstream>
#include <vector>
#include <ranges>
#include <set>
#include <regex>


std::vector<int> machine(int a, int b, int c, std::vector<int> instr) {
    std::vector<int> out;
    int ip = 0;
    auto combo = [&]() -> int{
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

void run_program(const std::string& filename) {
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
    std::vector<int> out = machine(
            std::stoi(match[1].str()),
            std::stoi(match[2].str()),
            std::stoi(match[3].str()),
            instr);
    std::cout << "output: ";
    for (int o: out) std::cout << o << ",";
    std::cout << std::endl;
}

int main() {
    run_program("example");
    run_program("input");
    return 0;
}
