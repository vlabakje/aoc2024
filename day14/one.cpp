#include <deque>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <regex>

struct Robot {
    int px, py, vx, vy;
};

std::vector<Robot> parse_robots(std::string& contents) {
    std::vector<Robot> out;
    std::regex pattern(R"(p=(-?\d+),(-?\d+) v=(-?\d+),(-?\d+))");
    std::smatch match;
    auto start = contents.cbegin();
    while(std::regex_search(start, contents.cend(), match, pattern)) {
        if (match.size() == 5) {
            out.push_back({
                std::stoi(match[1].str()), std::stoi(match[2].str()),
                std::stoi(match[3].str()), std::stoi(match[4].str())
            });
        } else {
            std::runtime_error("kapot!");
        }
        start = match.suffix().first;
    }
    return out;
}


int safety_factor(const std::string& filename, int seconds, int width, int height) {
	std::ifstream file(filename);
	if (!file) throw std::runtime_error("No such file: " + filename);
    
    std::string contents((std::istreambuf_iterator<char>(file)),
                          std::istreambuf_iterator<char>());
    
    auto robots = parse_robots(contents);

    std::map<std::pair<int, int>, int> robotcount;
    for (const auto& robot: robots) {
        int x = (robot.px + robot.vx * seconds) % width;
        if (x < 0) x += width;
        int y = (robot.py + robot.vy * seconds) % height;
        if (y < 0) y += height;
        robotcount[{x, y}]++;
        //std::cout << "robot at x=" << x << " y=" << y << " rc=" << robotcount[{x, y}] << std::endl;
    }

    /*
    for (int y = 0; y < height; ++ y) {
        for (int x = 0; x < width; ++x) {
            auto it = robotcount.find({x, y});
            std::cout << ((it != robotcount.end() and it->second > 0) ? std::to_string(it->second) : ".");
        }
        std::cout << std::endl;
    } // */
    
    // quadrants?
    int mid_x = width/2, mid_y = height / 2;
    int ul = 0, ur = 0, dl = 0, dr = 0;
    for (const auto& [coords, count] : robotcount) {
        if (coords.first < mid_x and coords.second < mid_y) ul += count;
        if (coords.first > mid_x and coords.second < mid_y) ur += count;
        if (coords.first < mid_x and coords.second > mid_y) dl += count;
        if (coords.first > mid_x and coords.second > mid_y) dr += count;
    }
    std::cout << "ul=" << ul << " ur=" << ur << " dl=" << dl << " dr=" << dr<< std::endl;
    return ul * ur * dl * dr;
}

int main() {
    std::cout << safety_factor("example", 100, 11, 7) << std::endl;
    std::cout << safety_factor("input", 100, 101, 103) << std::endl;
    return 0;
}
