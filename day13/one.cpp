#include <deque>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <regex>

struct Machine {
    int ax, ay, bx, by, px, py;
};

std::vector<Machine> machines_from_string(std::string& contents) {
    std::vector<Machine> out;
    std::regex pattern(R"(Button A: X\+(\d+), Y\+(\d+)\nButton B: X\+(\d+), Y\+(\d+)\nPrize: X=(\d+), Y=(\d+)\n)");
    std::smatch match;
    auto start = contents.cbegin();
    while(std::regex_search(start, contents.cend(), match, pattern)) {
        if (match.size() == 7) {
            out.push_back({
                std::stoi(match[1].str()), std::stoi(match[2].str()),
                std::stoi(match[3].str()), std::stoi(match[4].str()),
                std::stoi(match[5].str()), std::stoi(match[6].str())
            });
        } else {
            std::runtime_error("kapot!");
        }
        start = match.suffix().first;
    }
    return out;
}

int fewest_tokens(const Machine& machine){
    for (int a=1; a <= 100; ++a) {
        // can we use a to make a valid b
        if ((machine.px - (a * machine.ax)) % machine.bx != 0) continue;
        if ((machine.py - (a * machine.ay)) % machine.by != 0) continue;
        // calculate b
        int bx = (machine.px - (a * machine.ax)) / machine.bx;
        int by = (machine.py - (a * machine.ay)) / machine.by;
        if (bx == by and bx <= 100) {
            //std::cout << "a = " << a << " b=" << bx << std::endl;
            return a * 3 + bx;
        }
    }
    return 0;
}

int fewest_tokens_total(const std::string& filename) {
	std::ifstream file(filename);
	if (!file) throw std::runtime_error("No such file: " + filename);
    
    std::string contents((std::istreambuf_iterator<char>(file)),
                          std::istreambuf_iterator<char>());
    
    auto machines = machines_from_string(contents);

    int total = 0;
    for (const auto & machine: machines) {
        total += fewest_tokens(machine);
    }
    return total;
}

int main() {
    std::cout << fewest_tokens_total("example") << std::endl;
    std::cout << fewest_tokens_total("input") << std::endl;
    return 0;
}
