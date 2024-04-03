#include <catch2/catch_all.hpp>

#include <iostream>
#include <memory>

TEST_CASE("Dummy", "[dummy]")
{
    int a = 0;
    int b = 1;

    REQUIRE(a != b);
}

TEST_CASE("Section example", "[dummy][section]")
{
    int a = 0;
    int b = 1;
    std::cout << "before all sections: " << a << ", " << b << std::endl;

    SECTION("The first section") {
        REQUIRE(a != b);

        a = 10;
        b = a;
        std::cout << "inside the first section: " << a << ", " << b << std::endl;

        REQUIRE(a == b);
    }

    SECTION("The second section") {
        REQUIRE(a != b);

        b = 0;
        std::cout << "inside the second section: " << a << ", " << b << std::endl;

        REQUIRE(a == b);

        SECTION("A nested section #1") {
            std::cout << "on enter into nested #1: " << a << ", " << b << std::endl;

            a = 3;
            b = 4;

            REQUIRE(a != b);
        }

        SECTION("A nested section #2") {
            std::cout << "on enter into nested #2: " << a << ", " << b << std::endl;

            a = 5;
            b = 6;

            REQUIRE(a != b);
        }

        SECTION("A nested section #3") {
            std::cout << "on enter into nested #3: " << a << ", " << b << std::endl;

            a = 7;
            b = 8;

            REQUIRE(a != b);
        }
    }
}

TEST_CASE("Assertions for negation", "[require_false]")
{
    std::unique_ptr<int> d/*{ new int(10) }*/;
    REQUIRE_FALSE(d);
}

TEST_CASE("Assertions for exceptions", "[throws]")
{
    REQUIRE_NOTHROW([]{}());

    REQUIRE_THROWS(
        []{ throw std::runtime_error{ "Dummy" }; }());

    REQUIRE_THROWS_AS(
        []{ throw std::runtime_error{ "Dummy" }; }(),
        std::runtime_error);

    REQUIRE_THROWS_WITH(
        []{ throw std::runtime_error{ "Dummy" }; }(),
        "Dummy");

    REQUIRE_THROWS_WITH(
        []{ throw std::runtime_error{ "Message with Dummy inside" }; }(),
        Catch::Matchers::StartsWith("Message") &&
            Catch::Matchers::ContainsSubstring("Dummy"));
}

TEST_CASE("Assertions with matchers", "[require_that][matchers]")
{
    std::string value{ "This is a dummy value" };

    REQUIRE_THAT(value,
        Catch::Matchers::StartsWith("This") &&
        Catch::Matchers::ContainsSubstring("dummy") &&
        Catch::Matchers::EndsWith("value"));
}

