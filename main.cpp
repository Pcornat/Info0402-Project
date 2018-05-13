#include <iostream>
#include <set>
#include <chrono>
#include <random>

int main() {
	std::set<int, std::less<int>> s;
	std::default_random_engine generator(
			static_cast<unsigned long>(std::chrono::system_clock::now().time_since_epoch().count()));
	std::uniform_int_distribution<> distribution;
	std::random_device rd;
	const int max = 20;
	try {
		for (int i = 0; i < max; ++i) {
			s.insert(rd());
		}
	} catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	for (auto&& item : s) {
		std::cout << item << '\n';
	}
	return EXIT_SUCCESS;
}