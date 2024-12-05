#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>

struct Range {
    int start;
    int end;
};

void read_ranges(const std::string& filename, std::vector<Range>& ranges, std::vector<std::vector<int>>& pages) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("no such file: " + filename);
    }

    std::string line;
    bool in_ranges = true;
    while (std::getline(file, line)) {
        std::istringstream stream(line);
        if (line == "") {
            in_ranges = false;
        } else if (in_ranges) {
            std::string start, end;
            if (std::getline(stream, start, '|') && std::getline(stream, end, '|')) {
                ranges.push_back({std::stoi(start), std::stoi(end)});
            } else {
                std::cerr << "malformed line " << line << std::endl;
            }
        } else {
            std::vector<int> pagelist;
            std::string page_str;
            while (std::getline(stream, page_str, ',')) {
                pagelist.push_back(std::stoi(page_str));
            }
            pages.push_back(pagelist);
        }
    }
}

std::unordered_map<int, int> page_list_index_map(const std::vector<int>& page_list) {
    std::unordered_map<int, int> index_map;
    for (size_t i = 0; i < page_list.size(); ++i) {
        index_map[page_list[i]] = i;
    }
    return index_map;
}

bool adheres_to_all_ranges(const std::vector<Range>& ranges, std::unordered_map<int, int>& index_map) {
    for (const auto& range : ranges) {
        if (index_map.contains(range.start) and index_map.contains(range.end)) {
            if (index_map[range.start] > index_map[range.end]) {
                return false;
            }
        }
    }
    return true;
}

int total_middle_correct(const std::string& filename) {
    int total = 0;
    std::vector<Range> ranges;
    std::vector<std::vector<int>> pages;
    read_ranges(filename, ranges, pages);
    for (const auto& page_list: pages) {
        // part 1: make a mapping, loop through rules
        std::unordered_map<int, int> index_map = page_list_index_map(page_list);
        if (adheres_to_all_ranges(ranges, index_map)) {
            // std::cout << "found : " << page_list[0] << " " << page_list[page_list.size()/2] << std::endl;
            total += page_list[page_list.size()/2];
        }
    }
    return total;
}

int main() {
    std::cout << total_middle_correct("example") << std::endl;
    std::cout << total_middle_correct("input") << std::endl;
    return 0;
}
