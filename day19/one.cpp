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
        std::unordered_map<size_t, bool> cache;

    public:
        bool design_works(std::set<std::string>& parts, std::string& line, size_t max, size_t offset) {
            if (!cache.contains(offset)) {
                cache[offset] = this->_design_works(parts, line, max, offset);
            }
            return cache[offset];
        }

        bool _design_works(std::set<std::string>& parts, std::string& line, size_t max, size_t offset) {
            //for (size_t i = 1; i <= std::min(max, line.size()-offset); i++) {
            for (size_t i = 1; i <= max; i++) {
                if (parts.contains(line.substr(offset, i))) {
                    if (offset+i == line.size()) return true;
                    if (this->design_works(parts, line, max, offset+i)) {
                        return true;
                    }
                }
            }
            return false;
        }

};

int possible_designs(const std::string& filename) {
	std::ifstream file(filename);
	if (!file) throw std::runtime_error("No such file: " + filename);

    std::string line;
    std::getline(file, line);
    auto parts = parse_parts(line);
    std::getline(file, line);
    if (line.size() > 0) throw std::runtime_error("parse error");

    size_t max = 0;
    for (auto part : parts) max = std::max(max, part.size());

    int possible = 0;
    while (std::getline(file, line)) {
        MemoizedWorks mw;
        if (mw.design_works(parts, line, max, 0)) {
            possible++;
        }
    }
    return possible;
}

int main() {
    std::cout << possible_designs("example") << std::endl;
    std::cout << possible_designs("input") << std::endl;
    return 0;
}
