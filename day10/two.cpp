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

        std::vector<std::pair<int, int>> trailheads() {
            std::vector<std::pair<int, int>> out;
            for (int y=0; y<this->getRows(); ++y) {
                for (int x=0; x<this->getCols(); ++x) {
                    if ((*this)[x][y] - '0' == 0) {
                        out.push_back({x, y});
                    }
                }
            }
            return out;
        }

        int viable_from(int px, int py, int x, int y, int value) {
            if (value == 9) {
                //std::cout << "found 9 at x=" << x << " y=" << y << std::endl;
                return 1;
            }
            int total = 0;
            // look around x, y except for px, py for value+1
            std::vector<std::pair<int, int>> deltas = {{-1, 0}, {1, 0}, {0, 1}, {0, -1}};
            for (auto [dx, dy] : deltas) {
                if (x+dx == px and y+dy == py) continue;
                if (x+dx < 0 or x+dx >= this->getCols()) continue;
                if (y+dy < 0 or y+dy >= this->getRows()) continue;
                if ((*this)[x+dx][y+dy] - '0' == value + 1) {
                    total += viable_from(x, y, x+dx, y+dy, value+1);
                }
            }
            return total;
        }

        int sum_trailheads() {
            int total=0;
            for (auto [x, y] : this->trailheads()) {
                total += viable_from(-1, -1, x, y, 0);
                // std::cout << "x=" << x << " y=" << y << " nines=" << nines.size() << std::endl;
            }
            return total;
        }
};

int all_trailheads(const std::string& filename) {
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
    return grid.sum_trailheads();
}

int main() {
	std::cout << all_trailheads("example") << std::endl;
	std::cout << all_trailheads("input") << std::endl;
	return 0;
}
