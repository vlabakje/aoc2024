#include <algorithm>
#include <iostream>
#include <fstream>
#include <optional>
#include <map>
#include <vector>
#include <set>
#include <sstream>
#include <string>


std::vector<std::optional<int>> map_to_disk(std::string map){
    std::vector<std::optional<int>> out;
    for (size_t i=0; i<map.size(); ++i) {
        int id = i / 2;
        for (int j=0; j < map[i] - '0'; ++j) {
            if (i % 2 == 0) {
                out.push_back(id);
            } else {
                out.push_back(std::nullopt);
            }
        }
    }
    std::cout << "vector length: " << out.size() << std::endl;
    return out;
}

void defrag(std::vector<std::optional<int>>& disk) {
    int i = 0;
    int j = disk.size();
    while (i < j) {
        // move i forward until it hits a nullptr
        if (disk[i]) {
            i++;
        } else {
            std::swap(disk[i], disk[j]);
            j--;
        }
        // swap i and j, move j back
    }
}

uint64_t checksum(std::vector<std::optional<int>>& disk) {
    uint64_t out = 0;
    for (int i=0; i < int(disk.size()); ++i) {
        if (disk[i] != std::nullopt) {
            // std::cout << "out=" << out << " i=" << i << " disk[i]=" << *disk[i] << std::endl;
            out += i * *disk[i];
        }
    }
    return out;
}

void debug(std::vector<std::optional<int>>& disk) {
    std::cout << "debug: ";
    for (size_t i=0; i<disk.size(); ++i) {
        if (disk[i] == std::nullopt) {
            std::cout << ".";
        } else {
            std::cout << *disk[i];
        }
    }
    std::cout << std::endl;
}

uint64_t checksum(const std::string& filename) {
	std::ifstream file(filename, std::ios::in | std::ios::ate);
	if (!file) {
		throw std::runtime_error("No such file: " + filename);
	}

    // rediculously long way of saying gimme a string from a file but strip trailing whitespace pls.
    std::string input;
    input.resize(file.tellg());
    file.seekg(0);
    file.read(input.data(), input.size());
    input.erase(std::find_if(input.rbegin(), input.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), input.end());
    std::cout << "input length: " << input.size() << std::endl;
    
    auto disk = map_to_disk(input);
    // debug(disk);
    defrag(disk);
    // debug(disk);
    return checksum(disk);
}

int main() {
	std::cout << checksum("example") << std::endl;
	std::cout << checksum("example2") << std::endl;
	std::cout << checksum("input") << std::endl;
	return 0;
}
