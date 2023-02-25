//#include <iostream>
//#include <string>
//
//int main()
//{
//	std::string a = std::string('c', 5);
//
//	char c{};
//
//	std::cout << "Hello World!\n";
//}

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "CustomString/CustomString.h"
#include <vld.h>
#include <limits>

using String = CustomString<char>;

TEST_CASE("Test Custom String")
{
	String str{ "Hello World!" };

	SECTION("Test default ctor")
	{
		String string{};

		REQUIRE(string.Size() == 0);
		REQUIRE(string.Capacity() == 0);
		REQUIRE(string.MaxSize() == std::numeric_limits<size_t>::max());
		REQUIRE(string.Data() == nullptr);
	}

	SECTION("Test ctor with char and count")
	{
		String string{ 'L', 7 };

		REQUIRE(string.Size() == 8);
		REQUIRE(string.Capacity() >= 8);
		REQUIRE(string.MaxSize() == std::numeric_limits<size_t>::max());
		REQUIRE(string.Data() != nullptr);
		REQUIRE(string == "LLLLLLL");
	}

	SECTION("Test ctor with raw string")
	{
		REQUIRE(str.Size() == 13);
		REQUIRE(str.Capacity() >= 13);
		REQUIRE(str.MaxSize() == std::numeric_limits<size_t>::max());
		REQUIRE(str.Data() != nullptr);
	}

	SECTION("Compare against raw string")
	{
		REQUIRE(str == "Hello World!");
		REQUIRE(str != "Hello_World!");
		REQUIRE(str != "Hello World, This is Rhidian!");
		REQUIRE(str != "");
	}
}