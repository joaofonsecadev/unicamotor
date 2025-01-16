#include <catch2/catch_test_macros.hpp>

#include "core/memory.h"
#include "core/string.h"

TEST_CASE("UniquePtr lifetime handling and basic operations", "[uniqueptr]")
{
	char* raw = new char('A');
	{
		Unica::UniquePtr<char> char_unique_ptr(raw);
		REQUIRE(char_unique_ptr.Get() == raw);

		char* released_ptr = char_unique_ptr.Release();
		REQUIRE(released_ptr == raw);
		REQUIRE(char_unique_ptr.Get() == nullptr);

		char_unique_ptr.Reset(released_ptr);
		REQUIRE(char_unique_ptr.Get() == raw);

		Unica::UniquePtr<char> char_unique_ptr_2(Unica::CreateUniquePtr<char>('B'));
		REQUIRE(*char_unique_ptr_2.Get() == 'B');

		char_unique_ptr.Swap(char_unique_ptr_2);
		REQUIRE(*char_unique_ptr.Get() == 'B');
		REQUIRE(*char_unique_ptr_2.Get() == 'A');
	}
	REQUIRE(*raw != 'A');
}
