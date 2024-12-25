#include <deque>
#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <set>

struct Point{
    int x;
    int y;

    bool operator == (const Point& p) const {
        return (x == p.x) and (y == p.y);
    }
    
    bool operator < (const Point& p) const {
        return (x < p.x) or (x == p.x and y < p.y);
    }

    Point operator + (const Point& p) const {
        return Point(x+p.x, y+p.y);
    }
};

using P_score = std::pair<Point, int>;

constexpr std::array<Point, 4> deltas {
    Point( 1, 0),
    Point( 0, 1),
    Point(-1, 0),
    Point( 0,-1)
};

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

        void swap(int x1, int y1, int x2, int y2) {
            std::swap(grid[y1][x1], grid[y2][x2]);
        }

        bool valid(int x, int y) {
            return (x >= 0 and x < width and y >= 0 and y < height);
        }

        bool valid(const Point& p) {
            return valid(p.x, p.y);
        }

        void print() {
            for (const auto & line: grid) std::cout << line << std::endl;
        }
};

class Maze: public Grid {
    private:
        int start_x, start_y;
        int end_x, end_y;

    public:

        explicit Maze(int width, int height)
            : Grid(std::vector<std::string>(height, std::string(width, '.'))),
              start_x(0), start_y(0), end_x(width - 1), end_y(height - 1) {}

        int shortest_path() {
            auto compare = [](const P_score& a, const P_score& b) { return a.second > b.second; };
            std::priority_queue<P_score, std::vector<P_score>, decltype(compare)> pqueue;
            std::set<Point> visited;
            pqueue.push({Point(start_x, start_y), 0});
            while (!pqueue.empty()) {
                const auto [point, score] = pqueue.top();
                //std::cout << "p(x=" <<point.x << ", y=" << point.y << ") score=" << score << std::endl;
                pqueue.pop();
                if (visited.contains(point)) continue;
                visited.insert(point);
                if (point.x == end_x and point.y == end_y) return score;
                for (const auto& delta: deltas) {
                    auto nextpoint = point + delta;
                    if (valid(nextpoint) and at(nextpoint.x, nextpoint.y) != '#') {
                        pqueue.push({nextpoint, score + 1});
                    }
                }
            }
            return -1;
        }
};

int path(const std::string& filename, int width, int height) {
	std::ifstream file(filename);
	if (!file) throw std::runtime_error("No such file: " + filename);

    std::vector<Point> bytes;
	std::string line;
	while (std::getline(file, line)) {
        int x, y;
        std::string_view sv = line;
        auto commaPos = sv.find(',');
        std::from_chars(sv.data(), sv.data() + commaPos, x);
        std::from_chars(sv.data() + commaPos + 1, sv.data() + sv.size(), y);
        bytes.push_back(Point(x, y));
    }

    Maze maze = Maze(width, height);
    for (int i=0; i < static_cast<int>(bytes.size()); i++) {
        maze.update(bytes[i].x, bytes[i].y, '#');
        if (maze.shortest_path() == -1) {
            std::cout << bytes[i].x << "," << bytes[i].y << std::endl;
            return 0;
        }
    }
    return 0;
}

int main() {
    std::cout << path("example", 7, 7) << std::endl;
    std::cout << path("input", 71, 71) << std::endl;
    return 0;
}
