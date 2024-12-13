#include <deque>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>

class Grid {
    protected:
        std::vector<std::string> grid;
        int width;
        int height;

    public:
        explicit Grid(const std::vector<std::string>& g) : grid(g) {
            width = static_cast<int>(g[0].size());
            height = static_cast<int>(g.size());
        }
        char at(int x, int y) const {
            return grid[y][x];
        }

        void update(int x, int y, char c) {
            grid[y][x] = c;
        }

        bool valid(int x, int y) {
            return (x >= 0 and x < width and y >= 0 and y < height);
        }

        std::vector<std::pair<int, int>> neighbours(int x, int y) {
            std::vector<std::pair<int, int>> deltas = {{-1, 0}, {1, 0}, {0, 1}, {0, -1}};
            std::vector<std::pair<int, int>> out;
            for (auto [dx, dy]: deltas) {
                if (valid(x+dx, y+dy)) {
                    out.push_back({x+dx, y+dy});
                }
            }
            return out;
        }
};

class Garden: public Grid {
    private:
        std::set<std::pair<int, int>> visited;

    public:
        explicit Garden(const std::vector<std::string>& g) : Grid(g) {}

        int price(int x, int y) {
            int area = 0;
            int perimeter = 0;
            char c = at(x, y);
            std::deque<std::pair<int, int>> queue = {{x, y}};
            // bfs from x,y adding anything to visited and keeping track of area+perimeter
            while (!queue.empty()) {
                int neighbour_count = 0;
                auto [x, y] = queue.front();
                if (!visited.contains({x, y})) {
                    for (auto [nx, ny]: neighbours(x, y)) {
                        if (at(nx, ny) == c){
                            neighbour_count++;
                            if (!visited.contains({nx, ny})) queue.push_back({nx, ny});
                        }
                    }
                    area++;
                    perimeter += 4 - neighbour_count;
                    visited.insert({x, y});
                    // std::cout << "    x=" << x << " y=" << y << " area=" << area << " p=" << perimeter << std::endl;
                }
                queue.pop_front();
            }
            // std::cout << c << " x=" << x << " y=" << y << " area=" << area << " p=" << perimeter << std::endl;
            return area * perimeter;
        }

        int total_price() {
            int total = 0;
            for (int y=0; y < height; ++y) {
                for (int x=0; x < width; ++x) {
                    if (!visited.contains({x, y})) {
                        total += price(x, y);
                        // std::cout << "total now " << total << " x=" << x << " y=" << y << std::endl;
                    }
                }
            }
            return total;
        }

};

int grid_price(const std::string& filename) {
	std::ifstream file(filename);
	if (!file) {
		throw std::runtime_error("No such file: " + filename);
	}

	std::vector<std::string> lines;
	std::string line;
	while (std::getline(file, line)) {
		lines.push_back(line);
	}

	Garden garden = Garden(lines);
    return garden.total_price();
}

int main() {
    std::cout << grid_price("example") << std::endl;
    std::cout << grid_price("example2") << std::endl;
    std::cout << grid_price("input") << std::endl;
    return 0;
}
