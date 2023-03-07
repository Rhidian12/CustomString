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

	SECTION("Test Copy operator")
	{
		String str{ "Hello World!" };
		String string = str;
		String string2{ "This is a string!" };
		string2 = str;

		REQUIRE(string.Size() == str.Size());
		REQUIRE(string.Capacity() == str.Capacity());
		REQUIRE(string.MaxSize() == str.MaxSize());
		REQUIRE(string.Data() != str.Data());
		REQUIRE(string == str);
		REQUIRE(string == "Hello World!");
		REQUIRE(IsStringNullTerminated(str));
		REQUIRE(IsStringNullTerminated(string));

		REQUIRE(string2.Size() == str.Size());
		REQUIRE(string2.Capacity() == str.Capacity());
		REQUIRE(string2.MaxSize() == str.MaxSize());
		REQUIRE(string2.Data() != str.Data());
		REQUIRE(string2 == str);
		REQUIRE(string2 == "Hello World!");
		REQUIRE(IsStringNullTerminated(str));
		REQUIRE(IsStringNullTerminated(string2));
	}

	SECTION("Test Move Ctor")
	{
		String str{ "Hello World!" };
		String string{ std::move(str) };

		REQUIRE(string.Size() == 12);
		REQUIRE(string.Capacity() >= 12);
		REQUIRE(string.MaxSize() == std::numeric_limits<size_t>::max());
		REQUIRE(string.Data() != str.Data());
		REQUIRE(string == "Hello World!");
		REQUIRE(IsStringNullTerminated(string));

		REQUIRE(str.Size() == 0);
		REQUIRE(str.Capacity() == 0);
		REQUIRE(str.Data() == nullptr);
	}

	SECTION("Test Move Operator")
	{
		String str{ "Hello World!" };
		String string = std::move(str);

		REQUIRE(string.Size() == 12);
		REQUIRE(string.Capacity() >= 12);
		REQUIRE(string.MaxSize() == std::numeric_limits<size_t>::max());
		REQUIRE(string.Data() != str.Data());
		REQUIRE(string == "Hello World!");
		REQUIRE(IsStringNullTerminated(string));

		REQUIRE(str.Size() == 0);
		REQUIRE(str.Capacity() == 0);
		REQUIRE(str.Data() == nullptr);

		String string2{ "Some other string!" };
		string2 = std::move(string);

		REQUIRE(string2.Size() == 12);
		REQUIRE(string2.Capacity() >= 12);
		REQUIRE(string2.MaxSize() == std::numeric_limits<size_t>::max());
		REQUIRE(string2.Data() != str.Data());
		REQUIRE(string2 == "Hello World!");
		REQUIRE(IsStringNullTerminated(string2));

		REQUIRE(string.Size() == 0);
		REQUIRE(string.Capacity() == 0);
		REQUIRE(string.Data() == nullptr);
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

	SECTION("Adding a raw string to an existing string")
	{
		String str{ "Hello World!" };

		REQUIRE(str.Size() == 12);
		REQUIRE(str.Capacity() >= 12);
		REQUIRE(str.MaxSize() == std::numeric_limits<size_t>::max());
		REQUIRE(str.Data() != nullptr);
		REQUIRE(IsStringNullTerminated(str));

		str += " This is Rhidian!";

		REQUIRE(str.Size() == 29);
		REQUIRE(str.Capacity() >= 29);
		REQUIRE(str.MaxSize() == std::numeric_limits<size_t>::max());
		REQUIRE(str.Data() != nullptr);
		REQUIRE(str[12] != '\0');
		REQUIRE(str[12] == ' ');
		REQUIRE(str == "Hello World! This is Rhidian!");
		REQUIRE(IsStringNullTerminated(str));
	}

	SECTION("Adding another string to an existing string")
	{
		String str{ "Hello World!" };

		REQUIRE(str.Size() == 12);
		REQUIRE(str.Capacity() >= 12);
		REQUIRE(str.MaxSize() == std::numeric_limits<size_t>::max());
		REQUIRE(str.Data() != nullptr);
		REQUIRE(IsStringNullTerminated(str));

		String string{ " This is Rhidian!" };
		str += string;

		REQUIRE(str.Size() == 29);
		REQUIRE(str.Capacity() >= 29);
		REQUIRE(str.MaxSize() == std::numeric_limits<size_t>::max());
		REQUIRE(str.Data() != nullptr);
		REQUIRE(str[12] != '\0');
		REQUIRE(str[12] == ' ');
		REQUIRE(str == "Hello World! This is Rhidian!");
		REQUIRE(IsStringNullTerminated(str));
	}

	SECTION("Appending characters to a string")
	{
		String string{ "Hello World!" };

		string.Append('x', 5);

		REQUIRE(string.Size() == 17);
		REQUIRE(string.Capacity() >= 17);
		REQUIRE(string.Data() != nullptr);
		REQUIRE(string == "Hello World!xxxxx");
		REQUIRE(IsStringNullTerminated(string));
	}

	SECTION("Testing Substring")
	{
		String string{ "Hello World!" };

		REQUIRE(string.Substring(0) == "Hello World!");
		REQUIRE(string.Substring(0, 5) == "Hello");
		REQUIRE(string.Substring(6) == "World!");
		REQUIRE(string.Substring(2, 3) == "llo");
		REQUIRE(string.Substring(0, 1) == "H");
	}

	SECTION("Testing StartsWith")
	{
		String string{ "Hello World!" };
		String string2{ "World" };
		String string3{ "Hello" };

		REQUIRE(string.StartsWith("Hello"));
		REQUIRE(!string.StartsWith(string2));
		REQUIRE(string.StartsWith(string3));
		REQUIRE(string.StartsWith("Hello World!"));
		REQUIRE(!string.StartsWith("ello World!"));
		REQUIRE(!string.StartsWith("Hello Word"));
		REQUIRE(string.StartsWith("Hello World"));
	}

	SECTION("Testing EndsWith")
	{
		String string{ "Hello World!" };
		String string2{ "World!" };
		String string3{ "Hello" };

		REQUIRE(string.EndsWith("World!"));
		REQUIRE(string.EndsWith(string2));
		REQUIRE(!string.EndsWith(string3));
		REQUIRE(string.EndsWith("Hello World!"));
		REQUIRE(!string.EndsWith("Hello World"));
		REQUIRE(!string.EndsWith("Hello W!"));
		REQUIRE(string.EndsWith("!"));
	}

	SECTION("Testing IndexOf()")
	{
		String string{ "Hello World!" };
		String string2{ "Hello" };
		String string3{ " World!" };
		String string4{ " Not the same string." };
		String string5{ "Hello World! This is a longer string." };

		REQUIRE(string.IndexOf('H') == 0);
		REQUIRE(string.IndexOf('e') == 1);
		REQUIRE(string.IndexOf('l') == 2);
		REQUIRE(string.IndexOf('o') == 4);
		REQUIRE(string.IndexOf('!') == 11);
		REQUIRE(string.IndexOf('x') == String::NoPos);

		REQUIRE(string.IndexOf(string2) == 0);
		REQUIRE(string.IndexOf(string3) == 5);
		REQUIRE(string.IndexOf(string4) == String::NoPos);
		REQUIRE(string.IndexOf(string5) == String::NoPos);
		REQUIRE(string.IndexOf(string) == 0);

		REQUIRE(string.IndexOf("Hello") == 0);
		REQUIRE(string.IndexOf(" World!") == 5);
		REQUIRE(string.IndexOf(" World! xoxo") == String::NoPos);
		REQUIRE(string.IndexOf("Hello World!") == 0);
		REQUIRE(string.IndexOf("Hello_World!") == String::NoPos);
	}
}