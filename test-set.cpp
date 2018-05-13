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
	Set<unsigned int> mySet;
	std::set<unsigned int> stlSet;
	const int max = 15;
	std::random_device rd;
	REQUIRE(mySet.getSize() == stlSet.size());
	for (int i = 0; i < max; ++i) {
		unsigned int number = rd();
		mySet.insert(number);
		stlSet.insert(number);
	}//*/
	REQUIRE(mySet.getSize() == stlSet.size());
}