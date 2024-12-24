#include <deque>
#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <set>

struct Point{
    int x;
    int y;
    int d; // direction E=0, S=1, W=2, N=3

    bool operator == (const Point& p) const {
        return (x == p.x) and (y == p.y) and (d == p.d);
    }
    
    bool operator < (const Point& p) const {
        return (x < p.x) or (x == p.x and y < p.y) or (x == p.x and y == p.y and d < p.d);
    }

    Point operator + (const Point& p) const {
        return Point(x+p.x, y+p.y, p.d);
    }
};

using P_score = std::pair<Point, int>;

constexpr std::array<Point, 4> deltas {
    Point( 1, 0, 0),
    Point( 0, 1, 1),
    Point(-1, 0, 2),
    Point( 0,-1, 3)
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
        Point start;
        int end_x, end_y;

    public:
        explicit Maze(const std::vector<std::string>& g) : Grid(g) {
            for (int y=0; y<height; y++) {
                for (int x=0; x<width; x++) {
                    if (at(x, y) == 'E') {
                        end_x = x;
                        end_y = y;
                    } else if (at(x, y) == 'S') {
                        start = Point(x, y, 0);
                    }
                }
            }
        }

        std::vector<P_score> left_right_straight(const Point& p, int score) {
            std::vector<P_score> out;
            out.push_back({p + deltas[p.d], score + 1});          // straight on
            out.push_back({p + deltas[(p.d+3)%4], score + 1001}); // left
            out.push_back({p + deltas[(p.d+1)%4], score + 1001}); // right
            return out;
        }

        int shortest_path() {
            auto compare = [](const P_score& a, const P_score& b) { return a.second > b.second; };
            std::priority_queue<P_score, std::vector<P_score>, decltype(compare)> pqueue;
            std::set<Point> visited;
            pqueue.push({start, 0});
            while (!pqueue.empty()) {
                const auto [point, score] = pqueue.top();
                //std::cout << "p(x=" <<point.x << ", y=" << point.y << ") score=" << score << std::endl;
                pqueue.pop();
                if (visited.contains(point)) continue;
                visited.insert(point);
                if (point.x == end_x and point.y == end_y) return score;
                for (const auto& [nextpoint, nextscore] : left_right_straight(point, score)) {
                    //std::cout << "next p(x=" <<nextpoint.x << ", y=" << nextpoint.y << ", d=" << nextpoint.d << ") score=" << nextscore << std::endl;

                    if (valid(nextpoint) and at(nextpoint.x, nextpoint.y) != '#') {
                        pqueue.push({nextpoint, nextscore});
                    }
                }
            }
            return -1;
        }
};

int path(const std::string& filename) {
	std::ifstream file(filename);
	if (!file) throw std::runtime_error("No such file: " + filename);

	std::vector<std::string> lines;
	std::string line;
	while (std::getline(file, line)) {
        lines.push_back(line);
    }

    Maze maze = Maze(lines);
    return maze.shortest_path();
}

int main() {
    std::cout << path("example") << std::endl;
    std::cout << path("input") << std::endl;
    return 0;
}
