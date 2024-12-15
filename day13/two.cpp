#include <cmath>
#include <deque>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <regex>

struct Machine {
    int64_t ax, ay, bx, by, px, py;
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
                std::stoi(match[5].str())+10000000000000, 
                std::stoi(match[6].str())+10000000000000
            });
        } else {
            std::runtime_error("kapot!");
        }
        start = match.suffix().first;
    }
    return out;
}

int64_t fewest_tokens(const Machine& m){
    // A = (bx*y - by*x) / (bx*ay - by*ax)
    //    B = (x-ax*A) / bx
    //    if abs(A - round(A)) < tolerance and abs(B - round(B)) < tolerance:
    //        total += 3*A + B
    double a = (m.bx * m.py - m.by * m.px) / (m.bx * m.ay - m.by * m.ax);
    double b = (m.px - m.ax * a) / m.bx;
    if (std::floor(a) == a and std::floor(b) == b) {
        return int64_t(3 * a + b);
    }
    return 0;
}

int64_t fewest_tokens_total(const std::string& filename) {
	std::ifstream file(filename);
	if (!file) throw std::runtime_error("No such file: " + filename);
    
    std::string contents((std::istreambuf_iterator<char>(file)),
                          std::istreambuf_iterator<char>());
    
    auto machines = machines_from_string(contents);

    int64_t total = 0;
    for (const auto & machine: machines) {
        total += fewest_tokens(machine);
        if (total < 0) throw std::runtime_error("overflow");
    }
    return total;
}

int main() {
    //std::cout << fewest_tokens_total("example") << std::endl;
    std::cout << fewest_tokens_total("input") << std::endl;
    return 0;
}
