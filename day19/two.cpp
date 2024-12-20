#include <cmath>
#include <deque>
#include <iostream>
#include <fstream>
#include <vector>
#include <ranges>
#include <set>
#include <regex>
#include <unordered_map>

std::set<std::string> parse_parts(std::string& line) {
    std::set<std::string> out;
    size_t pos = 0;
    std::string part;
    while ((pos = line.find(", ")) != std::string::npos) {
        part = line.substr(0, pos);
        out.insert(part);
        line.erase(0, pos + 2);
    }
    out.insert(line);
    return out;
}

class MemoizedWorks {
    private:
        std::unordered_map<size_t, int64_t> c_ways;

    public:
        int64_t design_ways(std::set<std::string>& parts, std::string& line, size_t max, size_t offset) {
            if (!c_ways.contains(offset)) {
                c_ways[offset] = this->_design_ways(parts, line, max, offset);
            }
            return c_ways[offset];
        }

        int64_t _design_ways(std::set<std::string>& parts, std::string& line, size_t max, size_t offset) {
            int64_t ways = 0;
            for (size_t i = 1; i <= max; i++) {
                if (offset+i <= line.size()) {
                    if (parts.contains(line.substr(offset, i))) {
                        if (offset+i == line.size()) ways += 1;
                        else ways += this->design_ways(parts, line, max, offset+i);
                    }
                }
            }
            return ways;
        }
};

int64_t possible_designs(const std::string& filename) {
	std::ifstream file(filename);
	if (!file) throw std::runtime_error("No such file: " + filename);

    std::string line;
    std::getline(file, line);
    auto parts = parse_parts(line);
    std::getline(file, line);
    if (line.size() > 0) throw std::runtime_error("parse error");

    size_t max = 0;
    for (auto part : parts) max = std::max(max, part.size());

    int64_t possible = 0; 
    MemoizedWorks mw;
    while (std::getline(file, line)) {
        MemoizedWorks mw = MemoizedWorks();
        possible += mw.design_ways(parts, line, max, 0);
    }
    return possible;
}

int main() {
    std::cout << possible_designs("example") << std::endl;
    std::cout << possible_designs("input") << std::endl;
    return 0;
}
