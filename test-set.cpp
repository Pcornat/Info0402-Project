#include <catch.hpp>
#include <set>
#include <chrono>
#include "Set.hpp"

TEST_CASE("Test constructeur", "[1][constructeur test]") {
	std::set<int> stlSet;
	Set<int> mySet;
	REQUIRE(stlSet.size() == mySet.getSize());
}

TEST_CASE("Test insertion", "[2][test insertion]") {
	Set<int> mySet;
	std::set<int> stlSet;
	const int max = 15;
	std::default_random_engine generator(
			static_cast<unsigned long>(std::chrono::system_clock::now().time_since_epoch().count()));
	std::uniform_int_distribution<> distribution;
	REQUIRE(mySet.getSize() == stlSet.size());
	for (int i = 0; i < max; ++i) {
		int number = distribution(generator);
		mySet.insert(number);
		stlSet.insert(number);
	}//*/
	REQUIRE(mySet.getSize() == stlSet.size());
}