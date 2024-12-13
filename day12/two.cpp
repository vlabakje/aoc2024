#include <algorithm>
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
                out.push_back({x+dx, y+dy});
            }
            return out;
        }
};

class Garden: public Grid {
    private:
        std::set<std::pair<int, int>> visited;

    public:
        explicit Garden(const std::vector<std::string>& g) : Grid(g) {}

        void debug(std::vector<std::pair<int, int>>& o){
            for (const auto& c: o) {
                std::cout << "(" << c.first << ", " << c.second << ") ";
            }
            std::cout << std::endl;
        }

        int remove_from(std::vector<std::pair<int, int>>& othersides, int x, int y, int dx, int dy) {
            int removed = 0;
            std::pair<int, int> target = {x+dx, y+dy};
            auto it = std::find(othersides.begin(), othersides.end(), target);
            while (it != othersides.end()) {
                //std::cout << "remove (" << target.first << ", " << target.second << ")" << std::endl;
                othersides.erase(it);
                target.first += dx;
                target.second += dy;
                removed++;
                //std::cout << "target " << target.first << " " << target.second << " dx=" << dx << " dy=" << dy << std::endl;
                it = std::find(othersides.begin(), othersides.end(), target);
            }
            return removed;
        }
        
        int dedupe(std::vector<std::pair<int, int>>& othersides, int dx, int dy) {
            // de-dupe the othersides to figure out number of sides
            std::sort(othersides.begin(), othersides.end());
            int sides = 0;
            while (othersides.size()) {
                //debug(othersides);
                //std::cout << "othersides=" << othersides.size() << " sides=" << sides << std::endl;
                auto [x, y] = othersides.back();
                othersides.pop_back();
                remove_from(othersides, x, y, dx, dy);
                sides++;
            }
            return sides;
        }

        int price(int x, int y) {
            int area = 0;
            int perimeter = 0;
            char c = at(x, y);
            std::deque<std::pair<int, int>> queue = {{x, y}};
            std::vector<std::pair<int, int>> top_othersides;
            std::vector<std::pair<int, int>> bottom_othersides;
            std::vector<std::pair<int, int>> left_othersides;
            std::vector<std::pair<int, int>> right_othersides;
            // bfs from x,y adding anything to visited and keeping track of area+perimeter
            while (!queue.empty()) {
                int neighbour_count = 0;
                auto [x, y] = queue.front();
                if (!visited.contains({x, y})) {
                    for (auto [nx, ny]: neighbours(x, y)) {
                        if (valid(nx, ny) and at(nx, ny) == c){
                            neighbour_count++;
                            if (!visited.contains({nx, ny})) queue.push_back({nx, ny});
                        } else if (ny == y and nx-x == -1) {
                            left_othersides.push_back({nx, ny});
                        } else if (ny == y and nx-x == 1) {
                            right_othersides.push_back({nx, ny});
                        } else if (ny-y == 1) {
                            bottom_othersides.push_back({nx, ny});
                        } else {
                            top_othersides.push_back({nx, ny});
                        }
                    }
                    area++;
                    visited.insert({x, y});
                    // std::cout << "    x=" << x << " y=" << y << " area=" << area << " p=" << perimeter << std::endl;
                }
                queue.pop_front();
            }
            perimeter += dedupe(top_othersides, -1, 0);
            perimeter += dedupe(bottom_othersides, -1, 0);
            perimeter += dedupe(left_othersides, 0, -1);
            perimeter += dedupe(right_othersides, 0, -1);
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
    //std::cout << grid_price("example") << std::endl;
    //std::cout << grid_price("example-e") << std::endl;
    //std::cout << grid_price("example-mobius") << std::endl;
    //std::cout << grid_price("example2") << std::endl;
    //std::cout << grid_price("test") << std::endl;
    std::cout << grid_price("input") << std::endl;
    return 0;
}
