#include <catch2/catch_test_macros.hpp>
#include "core/memory.h"
#include "core/string.h"

TEST_CASE("Unique Ptr lifetime handling", "[unique_ptr]")
{
	Unica::UniquePtr<char> new_ptr;
	Unica::String test("o meu nome e rebeca");
}