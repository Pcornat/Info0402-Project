#include <catch.hpp>
#include <set>
#include "Set.hpp"

TEST_CASE("Test constructeur", "[1][constructeur test]") {
	std::set<int> stlSet;
	Set<int> mySet;
	REQUIRE(stlSet.size() == mySet.getSize());
}

TEST_CASE("Test insertion", "[2][insertion test]") {

}