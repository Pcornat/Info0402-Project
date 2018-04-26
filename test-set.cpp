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
	const int max = 5;
	default_random_engine generator(static_cast<unsigned long>(chrono::system_clock::now().time_since_epoch().count()));
	uniform_int_distribution<> distribution(15);
	REQUIRE(mySet.getSize() == stlSet.size());

	/*
	 * FUCK YOU THE INSERT FUNCTION AND ALL OF IT INSIDE.
	 * FUCK YOU THE BOOK THAT IS FALSE, YES I'M RAGING
	 * It segfaults sometimes, not always at the same moment, and some other times it does not.
	 */
	for (int i = 0; i < max; ++i) {
		int number = distribution(generator);
		mySet.insert(number);
		stlSet.insert(number);
	}//*/
	REQUIRE(mySet.getSize() == stlSet.size());
}