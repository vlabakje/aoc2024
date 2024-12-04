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
			for (size_t x=1; x<this->getCols()-1; ++x) {
				for (size_t y=1; y<this->getRows()-1; ++y) {
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

		bool mas_down(Point point) {
			if ((*this)[point.x-1][point.y-1] == 'M' and (*this)[point.x+1][point.y+1] == 'S') {
				return true;
			}
			if ((*this)[point.x-1][point.y-1] == 'S' and (*this)[point.x+1][point.y+1] == 'M') {
				return true;
			}
			return false;
		}

		bool mas_up(Point point) {
			if ((*this)[point.x+1][point.y-1] == 'M' and (*this)[point.x-1][point.y+1] == 'S') {
				return true;
			}
			if ((*this)[point.x+1][point.y-1] == 'S' and (*this)[point.x-1][point.y+1] == 'M') {
				return true;
			}
			return false;
		}

		int mas_around(Point point) {
			// is there a M/S around here?
			if (mas_down(point) and mas_up(point)) {
				return 1;
			}
			return 0;
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
	for (const auto& point: grid.filter_points('A')) {
		count += grid.mas_around(point);
	}
	return count;
}

int main() {
	std::cout << xmas_count("example") << std::endl;
	std::cout << xmas_count("input") << std::endl;
	return 0;
}
