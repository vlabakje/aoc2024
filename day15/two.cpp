#include <deque>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <ranges>

class Grid {
    protected:
        std::vector<std::string> grid;
        int width;
        int height;

    public:
        explicit Grid(const std::vector<std::string>& g) {
            for (const auto& line : g) {
                std::string buf;
                buf.reserve(line.size() * 2);
                for (char c: line) {
                    if (c == '@') {
                        buf.append(1, '@');
                        buf.append(1, '.');
                    } else if (c== 'O') {
                        buf.append(1, '[');
                        buf.append(1, ']');
                    } else {
                        buf.append(2, c);
                    }
                }
                grid.push_back(buf);
            }
            width = static_cast<int>(grid[0].size());
            height = static_cast<int>(grid.size());
        }

        char at(int x, int y) const {
            return grid[y][x];
        }

        void update(int x, int y, char c) {
            grid[y][x] = c;
        }

        void swap(int x1, int y1, int x2, int y2) {
            std::swap(grid[y1][x1], grid[y2][x2]);
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

        void print() {
            for (const auto & line: grid) std::cout << line << std::endl;
        }
};

class Warehouse: public Grid {
    private:
        std::set<std::pair<int, int>> visited;
        int robot_x, robot_y;

    public:
        explicit Warehouse(const std::vector<std::string>& g) : Grid(g) {
            for (int y=0; y<height; y++) {
                for (int x=0; x<width; x++) {
                    if (grid[y][x] == '@') {
                        robot_x = x;
                        robot_y = y;
                    }
                }
            }
        }

        bool push(int start_x, int start_y, int dx, int dy) {
            if (dy == 0) {
                int end_x = start_x;
                while (at(end_x + dx, start_y) == '[' or at(end_x + dx, start_y) == ']') {
                    end_x += dx;
                }
                if (at(end_x + dx, start_y) == '.') {
                    for (end_x += dx; end_x != start_x; end_x -= dx) {
                        swap(end_x, start_y, end_x - dx, start_y);
                    }
                    return true;
                }
                return false;
            }
            std::vector<std::pair<int, int>> move;
            std::set<std::pair<int, int>> push;
            push.insert({start_x, start_y});
            if (at(start_x, start_y) == '[') {
                push.insert({start_x+1, start_y});
            } else {
                push.insert({start_x-1, start_y});
            }

            while (!push.empty()) {
                std::set<std::pair<int, int>> next;
                for (const auto& [x, y] : push) {
                    move.push_back({x, y});
                    if (at(x, y+dy) == '#') return false;
                    if (at(x, y+dy) == '[') {
                        next.insert({x, y+dy});
                        next.insert({x+1, y+dy});
                    } else if (at(x, y+dy) == ']') {
                        next.insert({x, y+dy});
                        next.insert({x-1, y+dy});
                    } 
                }
                push = next;
            }
            // swap the contens of move, starting at the last element
            for (const auto& [tx, ty] : std::ranges::reverse_view(move)) {
                swap(tx, ty, tx, ty + dy);
            }
            return true;
        }

        void step(int dx, int dy) {
            bool can = false;
            if (at(robot_x+dx, robot_y+dy) == '.' ) {
                can = true;
            } else if (at(robot_x+dx, robot_y+dy) == '[' or at(robot_x+dx, robot_y+dy) == ']') {
                can = push(robot_x+dx, robot_y+dy, dx, dy);
            }
            if (can) {
                swap(robot_x, robot_y, robot_x+dx, robot_y+dy);
                robot_x += dx;
                robot_y += dy;
            }
        }

        void execute(std::string instructions) {
            for (const auto& instr : instructions) {
                if (instr == '<') step(-1, 0);
                if (instr == '>') step(1, 0);
                if (instr == '^') step(0, -1);
                if (instr == 'v') step(0, 1);
            }
        }

        int box_coords() {
            int out = 0;
            for (int y=0; y<height; y++) {
                for (int x=0; x<width; x++) {
                    if (at(x, y) == '[') {
                        out += 100 * y + x;
                    }
                }
            }
            return out;
        }
};

int all_boxes(const std::string& filename) {
	std::ifstream file(filename);
	if (!file) throw std::runtime_error("No such file: " + filename);

	std::vector<std::string> lines;
	std::string line;
	while (std::getline(file, line)) {
        if (line.size() == 0) break;
        lines.push_back(line);
    }

    std::string instructions;
	while (std::getline(file, line)) instructions += line;

    Warehouse warehouse = Warehouse(lines);
    warehouse.execute(instructions);
    return warehouse.box_coords();
}

int main() {
    //std::cout << all_boxes("small2") << std::endl;
    std::cout << all_boxes("example") << std::endl;
    std::cout << all_boxes("input") << std::endl;
    return 0;
}
