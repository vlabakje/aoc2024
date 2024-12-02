#include <algorithm>
#include <iostream>
#include <fstream>
#include <optional>
#include <vector>
#include <sstream>
#include <string>

template <typename T>
class AllButOneVector : public std::vector<T> {
	private:
		std::optional<size_t> skip;

		size_t adjustedIndex(size_t index) const {
			if (skip && index >= *skip) {
				return index + 1;
			}
			return index;
		}

	public:
		// constuctor with optional skipindex
		AllButOneVector(std::optional<size_t> s = std::nullopt) : skip(s) {}

		void setSkip(std::optional<size_t> s) {
			skip = s;
		}

		std::optional<size_t> getSkip() const {
			return skip;
		}

		// element access
		T& operator[](size_t index) {
			return std::vector<T>::operator[](adjustedIndex(index));
		}

		const T& operator[](size_t index) const {
			return std::vector<T>::operator[](adjustedIndex(index));
		}

		// size override
		size_t size() const {
			return std::vector<T>::size() - (skip ? 1 : 0);
		}
};

void reportsFromFile(const std::string& filename, std::vector<int>& left, std::vector<int>& right) {
	std::ifstream file(filename);
	if (!file) {
		throw std::runtime_error("No such file: " + filename);
	}

	std::string line;
	while (std::getline(file, line)) {
		std::istringstream stream(line);
		int first, second;
		if (stream >> first >> second) {
			left.push_back(first);
			right.push_back(second);
		} else {
			throw std::runtime_error("unsupported format: " + line);
		}
	}
	file.close();
}

bool safe(AllButOneVector<int>& levels) {
	int c = levels[0];
	bool up = levels[1] - levels[0] > 0;
	for (size_t i = 1; i < levels.size(); ++i) {
		if ((abs(levels[i] - c) > 3) or (levels[i] == c)) {
			return false;
		}
		if ((up and (levels[i] < c)) or (!up and (levels[i] > c))){
			return false;
		}
		c = levels[i];
	}
	return true;
}

bool safe_one_removed(AllButOneVector<int>& levels) {
	for (size_t i = 0; i < levels.size()+1; ++i) {
		levels.setSkip(i);
		if (safe(levels)) {
			return true;
		}
	}
	return false;
}

int safe_count(const std::string& filename) {
	int count = 0;
	std::ifstream file(filename);
	if (!file) {
		throw std::runtime_error("No such file: " + filename);
	}

	std::string line;
	while (std::getline(file, line)) {
		std::istringstream stream(line);
		AllButOneVector<int> levels;
		int n;

		while (stream >> n) {
			levels.push_back(n);
		}

		if (safe(levels) or safe_one_removed(levels)) {
			count += 1;
		}
	}
	return count;
}

int main() {
	std::cout << safe_count("example") << std::endl;
	std::cout << safe_count("input") << std::endl;
	return 0;
}
