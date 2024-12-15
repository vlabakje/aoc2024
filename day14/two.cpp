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

void display(const std::map<std::pair<int, int>, int>& robotcount, int width, int height) {
    for (int y = 0; y < height; ++ y) {
        for (int x = 0; x < width; ++x) {
            auto it = robotcount.find({x, y});
            std::cout << ((it != robotcount.end() and it->second > 0) ? std::to_string(it->second) : ".");
        }
        std::cout << std::endl;
    }
}

int safety_factor(const std::string& filename, int width, int height) {
	std::ifstream file(filename);
	if (!file) throw std::runtime_error("No such file: " + filename);
    
    std::string contents((std::istreambuf_iterator<char>(file)),
                          std::istreambuf_iterator<char>());
    
    auto robots = parse_robots(contents);

    for (int seconds = 0; seconds < 10000; ++seconds) {
        std::map<std::pair<int, int>, int> robotcount;
        for (const auto& robot: robots) {
            int x = (robot.px + robot.vx * seconds) % width;
            if (x < 0) x += width;
            int y = (robot.py + robot.vy * seconds) % height;
            if (y < 0) y += height;
            robotcount[{x, y}]++;
        }
        std::cout << "====== " << seconds << " ====" << std::endl;
        display(robotcount, width, height);
    }
    return 0;
}

int main() {
    std::cout << safety_factor("input", 101, 103) << std::endl;
    return 0;
}
