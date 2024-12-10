#include <algorithm>
#include <iostream>
#include <fstream>
#include <optional>
#include <map>
#include <vector>
#include <sstream>
#include <set>
#include <string>
#include <utility>

class Grid {
	private:
		std::vector<std::string> grid;
		// helper class for grid[x][y] access
		class GridColumn {
			private:
				std::vector<std::string> grid;
				size_t col;

			public:
				GridColumn(std::vector<std::string> g, size_t c) : grid(g), col(c) {}

				// operator overload
				char & operator[](size_t row) {
					if (row >= grid.size()) {
						throw std::out_of_range("row out of bounds");
					}
					if (col >= grid[0].size()) {
						throw std::out_of_range("col out of bounds");
					}
					return grid[row][col];
				}
		};

		class ConstGridColumn {
			private:
				const std::vector<std::string> grid;
				size_t col;

			public:
				ConstGridColumn(const std::vector<std::string>& g, size_t c) : grid(g), col(c) {}

				// operator overload
				const char & operator[](size_t row) const {
					if (row >= grid.size()) {
						throw std::out_of_range("row out of bounds");
					}
					if (col >= grid[0].size()) {
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
				if (line.size() != this->getCols()) {
					throw std::invalid_argument("line lenghts not equal");
				}
			}
		}

		size_t getRows() const {
			return grid.size();
		}

		size_t getCols() const {
			return grid[0].size();
		}

		// element access
		GridColumn operator[](size_t col) {
			return GridColumn(grid, col);
		}

		ConstGridColumn operator[](size_t col) const {
			return ConstGridColumn(grid, col);
		}

        std::pair<int, int> start() {
            for (size_t x=0; x < this->getCols(); ++x) {
                for (size_t y=0; y< this->getRows(); ++y) {
                    if ((*this)[x][y] == '^') {
                        return {x, y};
                    }
                }
            }
            throw std::runtime_error("starting point not found");
        }

        void print() {
            for (const auto& l: grid) {
                std::cout << l << std::endl;
            }
        }

        int check_loop(int ox, int oy, int odx, int ody) {
            // WORST LOOP DETECT EVAH
            std::map<std::pair<int, int>, int> loops;
            grid[oy+ody][ox+odx] = '#';
            //this->print();
            std::cout << "checking loop x=" << ox << " y=" << oy;
            // temp overwrite grid
            int x = ox;
            int y = oy;
            int dx = odx;
            int dy = ody;
            while (not off_edge(x, y, dx, dy)) {
                if ((*this)[x+dx][y+dy] == '.' or (*this)[x+dx][y+dy] == '^') {
                    x = x+dx;
                    y = y+dy;
                    loops[{x, y}]++;
                } else if ((*this)[x+dx][y+dy] == '#') {
                    int nx = -dy;
                    int ny = dx;
                    dx = nx;
                    dy = ny;
                } else {
                    throw std::runtime_error("unexpected value in grid " + (*this)[x+dx][y+dy]);
                }
                if (loops[{x, y}] > 3) {
                    grid[oy+ody][ox+odx] = '.';
                    std::cout << " loopyloop" << std::endl;
                    return 1;
                }
            }
            grid[oy+ody][ox+odx] = '.';
            std::cout << " no loop." << std::endl;
            return 0;
        }

        int guard_points() {
            std::set<std::pair<int, int>> visited;
            auto [x, y] = start();
            visited.insert({x, y});
            int dx = 0;
            int dy = -1;
            int extra_block_loops = 0;
            while (not off_edge(x, y, dx, dy)) {
                // take step and add to visited
                if ((*this)[x+dx][y+dy] == '.' or (*this)[x+dx][y+dy] == '^') {
                    // put an extra block in and check for loops
                    if (!visited.contains({x+dx, y+dy})) {
                        extra_block_loops += check_loop(x, y, dx, dy);
                    }
                    x = x+dx;
                    y = y+dy;
                    visited.insert({x, y});
                    //std::cout << "visited " << x << " " << y << " " << dx << " " << dy << std::endl;
                } else if ((*this)[x+dx][y+dy] == '#') {
                    int nx = -dy;
                    int ny = dx;
                    dx = nx;
                    dy = ny;
                } else {
                    throw std::runtime_error("unexpected value in grid " + (*this)[x+dx][y+dy]);
                }
            }
            return extra_block_loops;
        }

        bool off_edge(int x, int y, int dx, int dy){
            if (x+dx < 0 or y+dy < 0){
                return true;
            }
            if (x+dx >= static_cast<int>(this->getCols()) or y+dy >= static_cast<int>(this->getRows())) {
                return true;
            }
			return false;
        }
};

int path_count(const std::string& filename) {
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
    return grid.guard_points();
}

int main() {
	std::cout << path_count("example") << std::endl;
	std::cout << path_count("input") << std::endl;
	return 0;
}