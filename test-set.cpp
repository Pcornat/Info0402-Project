#include <catch.hpp>
#include <set>
#include "Set.hpp"

TEST_CASE("Test constructeur", "[1][constructeur test]") {
	std::set<int> stlSet;
	Set<int> mySet;
	REQUIRE(stlSet.size() == mySet.getSize());
}

TEST_CASE("Test insertion", "[2][insertion test]") {
	Set<int> mySet;
	REQUIRE(mySet.getSize() == 0);
	for (int i = 1 ; i < 11 ; ++i) {
		mySet.insert(i);
	}
	REQUIRE(*(mySet.find(5)) == 10);
}