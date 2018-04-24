#include <catch.hpp>
#include <set>
#include <chrono>
#include "Set.hpp"

TEST_CASE("Test constructeur", "[1][constructeur test]") {
	set<int> stlSet;
	Set<int> mySet;
	REQUIRE(stlSet.size() == mySet.getSize());
}

TEST_CASE("Test insertion lvalue", "[2][test insertion lvalue]") {
	Set<int> mySet;
	set<int> stlSet;
	default_random_engine generator(static_cast<unsigned long>(chrono::system_clock::now().time_since_epoch().count()));
	uniform_int_distribution<> distribution(15);
	REQUIRE(mySet.getSize() == stlSet.size());

	for (int i = 0; i < 11; ++i) {
		int number = distribution(generator);
		mySet.insert(number);
		stlSet.insert(number);
	}//*/
	REQUIRE(mySet.getSize() == stlSet.size());
}