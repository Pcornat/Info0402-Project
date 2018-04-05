#include "test-set.hpp"

#include "Set.hpp"

TEST_CASE("Test constructeur", "[1][constructeur test]") {
	std::set<int, std::greater<int>> stlSet;
	Set<int, std::greater<int>> mySet;

	REQUIRE(stlSet.size() == mySet.getSize());
}