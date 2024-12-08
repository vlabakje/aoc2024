#include <algorithm>
#include <iostream>
#include <fstream>
#include <optional>
#include <map>
#include <vector>
#include <set>
#include <sstream>
#include <string>

class Grid {
	private:
		std::vector<std::string> grid;
		// helper class for grid[x][y] access
		class GridColumn {
			private:
				std::vector<std::string> grid;
				int col;

			public:
				GridColumn(std::vector<std::string> g, int c) : grid(g), col(c) {}

				// operator overload
				char & operator[](int row) {
					if (row >= int(grid.size())) {
						throw std::out_of_range("row out of bounds");
					}
					if (col >= int(grid[0].size())) {
						throw std::out_of_range("col out of bounds");
					}
					return grid[row][col];
				}
		};

		class ConstGridColumn {
			private:
				const std::vector<std::string> grid;
				int col;

			public:
				ConstGridColumn(const std::vector<std::string>& g, int c) : grid(g), col(c) {}

				// operator overload
				const char & operator[](int row) const {
					if (row >= int(grid.size())) {
						throw std::out_of_range("row out of bounds");
					}
					if (col >= int(grid[0].size())) {
						throw std::out_of_range("col out of bounds");
					}
					return grid[row][col];
				}
		};
	public:
		// constructor from vector of lines
		Grid(const std::vector<std::string>& lines) {
			grid = lines;
			if (lines.empty()) {
				throw std::invalid_argument("empty grid :(");
			}
			for (const auto& line: lines) {
				if (int(line.size()) != this->getCols()) {
					throw std::invalid_argument("line lenghts not equal");
				}
			}
		}

		int getRows() const {
			return grid.size();
		}

		int getCols() const {
			return grid[0].size();
		}

		// element access
		GridColumn operator[](int col) {
			return GridColumn(grid, col);
		}

		ConstGridColumn operator[](int col) const {
			return ConstGridColumn(grid, col);
		}

        std::vector<std::pair<int, int>> filter_points(char needle) {
			std::vector<std::pair<int, int>> points;
			for (int x=0; x<this->getCols(); ++x) {
				for (int y=0; y<this->getRows(); ++y) {
					if ((*this)[x][y] == needle) {
						points.push_back({x, y});
					}
				}
			}
			return points;
		}
        
        std::map<char, std::vector<std::pair<int, int>>> map_antennas() {
            std::map<char, std::vector<std::pair<int, int>>> out;
			for (int x=0; x<this->getCols(); ++x) {
				for (int y=0; y<this->getRows(); ++y) {
                    if ((*this)[x][y] != '.') {
                        out[(*this)[x][y]].push_back({x, y});
                    }
                }
            }
            return out;
        }

        std::string debug(const std::pair<int, int>& d) {
            return "(" + std::to_string(d.first) + ", " + std::to_string(d.second) + ")";
        }

        bool valid(std::pair<int, int>& d) {
            if (d.first < 0 or d.first >= this->getCols()) {
                return false;
            }
            if (d.second < 0 or d.second >= this->getRows()) {
                return false;
            }
            return true;
        }

        int antinode_count() {
            // step 1: dict of char -> antennas
            auto freq_antennas = this->map_antennas();
            std::set<std::pair<int, int>> antinodes;
            for (const auto& [freq, antennas]: freq_antennas) {
                for (size_t a=0; a < antennas.size() - 1; ++a) {
                    for (size_t b=a+1; b < antennas.size(); ++b) {
                        // step 2: for every char pair create antinodes
                        //std::cout << freq << " " + debug(antennas[a]) << " " + debug(antennas[b]) << std::endl;
                        int dx = antennas[b].first - antennas[a].first;
                        int dy = antennas[b].second - antennas[a].second;
                        std::pair<int, int> one = {antennas[a].first - dx, antennas[a].second - dy};
                        std::pair<int, int> two = {antennas[b].first + dx, antennas[b].second + dy};
                        if (valid(one)) {
                            //std::cout << " -> " + debug(one) << std::endl;
                            antinodes.insert(one);
                        }
                        if (valid(two)) {
                            //std::cout << " -> " + debug(two) << std::endl;
                            antinodes.insert(two);
                        }
                    }
                }
            }
            return antinodes.size();
        }
};

int antinode_count(const std::string& filename) {
	std::ifstream file(filename);
	if (!file) {
		throw std::runtime_error("No such file: " + filename);
	}

	std::vector<std::string> lines;
	std::string line;
	while (std::getline(file, line)) {
		lines.push_back(line);
	}

	Grid grid = Grid(lines);
    return grid.antinode_count();
}

int main() {
	std::cout << antinode_count("example") << std::endl;
	std::cout << antinode_count("input") << std::endl;
	return 0;
}
