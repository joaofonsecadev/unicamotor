#include <catch2/catch_test_macros.hpp>
#include "catch2/catch_template_test_macros.hpp"

#include "core/vector.h"

#include "core/string.h"
#include "motor/unicaconfig.h"

TEMPLATE_TEST_CASE("Vector lifetime handling", "[vector][template]", Unica::String)
{
    constexpr size_t vector_capacity = 2;

    SECTION("Default constructor")
    {
        Unica::Vector<TestType> vec;
        REQUIRE(vec.GetData() == nullptr);
    }

    SECTION("Initial and max capacity constructor")
    {
        Unica::Vector<TestType> vec(vector_capacity, vector_capacity);
        REQUIRE(vec.GetCapacity() == vector_capacity);
        REQUIRE(vec.GetData() != nullptr);
        vec.~Vector();
        REQUIRE(vec.GetCapacity() == 0);
        REQUIRE(vec.GetData() == nullptr);
    }
}

TEMPLATE_TEST_CASE("Vector pushback and resizing", "[vector][template]", Unica::String)
{
    SECTION("Unbounded")
    {
        Unica::Vector<TestType> vec;
        REQUIRE(vec.GetData() == nullptr);

        TestType elem = TestType();

        for (uint8_t i = 0; i < 15; i++)
        {
            vec.PushBack(elem);
        }
        REQUIRE(vec.GetData() != nullptr);
        REQUIRE(vec.GetSize() == 15);
        REQUIRE(vec.GetCapacity() == 16);

        vec.PushBack(elem);
        REQUIRE(vec.GetSize() == 16);
        REQUIRE(vec.GetCapacity() == 16);

        vec.PushBack(elem);
        REQUIRE(vec.GetSize() == 17);
        REQUIRE(vec.GetCapacity() == 32);
    }

    SECTION("Bounded")
    {
        Unica::Vector<TestType> vec(0, 16);
        REQUIRE(vec.GetData() == nullptr);

        TestType elem = TestType();

        for (uint8_t i = 0; i < 15; i++)
        {
            vec.PushBack(elem);
        }
        REQUIRE(vec.GetData() != nullptr);
        REQUIRE(vec.GetSize() == 15);
        REQUIRE(vec.GetCapacity() == 16);

        vec.PushBack(elem);
        REQUIRE(vec.GetSize() == 16);
        REQUIRE(vec.GetCapacity() == 16);

        REQUIRE(vec.PushBack(elem) == nullptr);
        REQUIRE(vec.GetSize() == 16);
        REQUIRE(vec.GetCapacity() == 16);
    }
}