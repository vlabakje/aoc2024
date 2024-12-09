#include <algorithm>
#include <iostream>
#include <fstream>
#include <optional>
#include <map>
#include <vector>
#include <set>
#include <sstream>
#include <string>

std::vector<std::optional<int>> map_to_disk(std::string map, std::map<int, int>& id_to_size, std::map<int, int>& id_to_start){
    std::vector<std::optional<int>> out;
    for (size_t i=0; i<map.size(); ++i) {
        int id = i / 2;
        if (i % 2 == 0) {
            id_to_size[id] = map[i] - '0';
            id_to_start[id] = out.size()-1;
        }
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

void defrag(std::vector<std::optional<int>>& disk, std::map<int, int>& id_to_size, std::map<int, int>& id_to_start) {
    for (auto it = id_to_start.rbegin(); it != id_to_start.rend(); ++it) {
        // this is the id and where it starts, the size is in id_to_size, search for a free spot
        //std::cout << "id=" << it->first << " offset=" << it->second << " size=" << id_to_size[it->first] << std::endl;
        int free_size = 0;
        for (int i=0; i<it->second+1; ++i) {
            // look for continuous free space of size id_to_size[it->first]
            if (disk[i] == std::nullopt) {
                free_size += 1;
                if (free_size == id_to_size[it->first]) {
                    // move this one over, break out
                    for (int j=1; j<=free_size; j++) {
                        std::swap(disk[i-free_size+j], disk[it->second+j]);
                        //std::cout << "swap: " << i-j << "<->" << it->second+j+1 << std::endl;
                    }
                    break;
                }
            } else {
                free_size = 0;
            }
            // we didn´t move it... too bad
        }
    }
    return;
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
    //std::cout << "input length: " << input.size() << std::endl;
    
    std::map<int, int> id_to_size;
    std::map<int, int> id_to_start;
    auto disk = map_to_disk(input, id_to_size, id_to_start);
    //debug(disk);
    defrag(disk, id_to_size, id_to_start);
    //debug(disk);
    return checksum(disk);
}

int main() {
	//std::cout << checksum("example") << std::endl;
	std::cout << checksum("example2") << std::endl;
	std::cout << checksum("input") << std::endl;
	return 0;
}
