//#include <iostream>
//#include <string>
//
//int main()
//{
//	std::string a = "cccc";
//
//	std::cout << a.size() << "\n";
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

bool IsStringNullTerminated(const String& str)
{
	return *(str.Data() + str.Size()) == '\0';
}

TEST_CASE("Test Custom String")
{
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

		REQUIRE(string.Size() == 7);
		REQUIRE(string.Capacity() >= 7);
		REQUIRE(string.MaxSize() == std::numeric_limits<size_t>::max());
		REQUIRE(string.Data() != nullptr);
		REQUIRE(string == "LLLLLLL");
		REQUIRE(string[0] == 'L');
		REQUIRE(string[6] == 'L');
		REQUIRE(IsStringNullTerminated(string));
	}

	SECTION("Test ctor with raw string")
	{
		String str{ "Hello World!" };

		REQUIRE(str.Size() == 12);
		REQUIRE(str.Capacity() >= 12);
		REQUIRE(str.MaxSize() == std::numeric_limits<size_t>::max());
		REQUIRE(str.Data() != nullptr);
		REQUIRE(IsStringNullTerminated(str));
	}

	SECTION("Test Copy Ctor")
	{
		String str{ "Hello World!" };
		String string{ str };

		REQUIRE(string.Size() == str.Size());
		REQUIRE(string.Capacity() == str.Capacity());
		REQUIRE(string.MaxSize() == str.MaxSize());
		REQUIRE(string.Data() != str.Data());
		REQUIRE(string == str);
		REQUIRE(string == "Hello World!");
		REQUIRE(IsStringNullTerminated(str));
		REQUIRE(IsStringNullTerminated(string));
	}

	SECTION("Compare against raw string")
	{
		String str{ "Hello World!" };

		REQUIRE(str == "Hello World!");
		REQUIRE(str != "Hello_World!");
		REQUIRE(str != "Hello World, This is Rhidian!");
		REQUIRE(str != "");
	}

	SECTION("Compare against other string")
	{
		String str{ "Hello World!" };
		String string{ "Hello!" };
		String string2{};

		REQUIRE(str == str);
		REQUIRE(str != string);
		REQUIRE(str != string2);

		REQUIRE(string == string);
		REQUIRE(string != str);
		REQUIRE(string != string2);

		REQUIRE(string2 == string2);
		REQUIRE(string2 != str);
		REQUIRE(string2 != string);
	}

	SECTION("Test ToUpper()")
	{
		String str{ "Hello World!" };
		String string = str.ToUpper();
		String string2{ "THIS_IS_A_TEST!" };
		string2.ToUpper();

		REQUIRE(str == "HELLO WORLD!");
		REQUIRE(string == "HELLO WORLD!");
		REQUIRE(string2 == "THIS_IS_A_TEST!");
		REQUIRE(string.Size() == str.Size());
		REQUIRE(string.Capacity() == str.Capacity());
		REQUIRE(string.MaxSize() == str.MaxSize());
		REQUIRE(string.Data() != str.Data());
		REQUIRE(IsStringNullTerminated(str));
		REQUIRE(IsStringNullTerminated(string));
		REQUIRE(IsStringNullTerminated(string2));
	}

	SECTION("Test ToLower()")
	{
		String str{ "Hello World!" };
		String string = str.ToLower();
		String string2{ "this_is_a_test!" };
		string2.ToLower();

		REQUIRE(str == "hello world!");
		REQUIRE(string == "hello world!");
		REQUIRE(string2 == "this_is_a_test!");
		REQUIRE(string.Size() == str.Size());
		REQUIRE(string.Capacity() == str.Capacity());
		REQUIRE(string.MaxSize() == str.MaxSize());
		REQUIRE(string.Data() != str.Data());
		REQUIRE(IsStringNullTerminated(str));
		REQUIRE(IsStringNullTerminated(string));
		REQUIRE(IsStringNullTerminated(string2));
	}
}