#include <algorithm>
#include <iostream>
#include <fstream>
#include <optional>
#include <vector>
#include <sstream>
#include <string>

class Point {
	public:
		size_t x;
		size_t y;

		Point(size_t x_ = 0, size_t y_ = 0) : x(x_), y(y_) {}
		
		friend std::ostream& operator<<(std::ostream& os, const Point& p) {
			os << "Point(" << p.x << "," << p.y << ")";
			return os;
		}
};

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

		std::vector<Point> filter_points(char needle) {
			std::vector<Point> points;
			for (size_t x=0; x<this->getCols(); ++x) {
				for (size_t y=0; y<this->getRows(); ++y) {
					if ((*this)[x][y] == needle) {
						points.push_back(Point(x, y));
					}
				}
			}
			return points;
		}

		bool search_from(Point point, int dx, int dy, const std::string match) {
			int x = point.x;
			int y = point.y;
			for (int i=1; i<4; ++i) {
				x += dx;
				y += dy;
				if (x < 0 or y < 0){
					return false;
				}
				if (x >= static_cast<int>(this->getCols()) or y >= static_cast<int>(this->getRows())) {
					return false;
				}
				if ((*this)[x][y] != match[i-1]) {
					return false;
				}
			}
			return true;
		}

		int xmas_from(Point point) {
			// given a point, look in all directions and return the number of XMAS matches
			int count = 0;
			for (int dx=-1; dx <2; ++dx) {
				for (int dy=-1; dy<2; ++dy) {
					if (dx == 0 and dy == 0) {
						continue;
					}
					if (search_from(point, dx, dy, "MAS")) {
						count += 1;
					}
				}
			}
			return count;
		}

};

int xmas_count(const std::string& filename) {
	int count = 0;
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
	for (const auto& point: grid.filter_points('X')) {
		count += grid.xmas_from(point);
	}
	return count;
}

int main() {
	std::cout << xmas_count("example") << std::endl;
	std::cout << xmas_count("input") << std::endl;
	return 0;
}
