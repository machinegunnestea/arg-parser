#include <catch2/catch_all.hpp>
#include <args_parse/args.hpp>
#include <args_parse/validator.hpp>
#include <iostream>
#include <memory>
#include <unordered_map>

TEST_CASE("Validation of new bool argument") {
    args_parse::Validator validator;
    args_parse::BoolArg arg('b',"bool");
    REQUIRE(validator.validateNewArgument(&arg) == true);
}
TEST_CASE("Validation of new int argument") {
    args_parse::Validator validator;
    args_parse::IntArg arg('i', "int");
    REQUIRE(validator.validateNewArgument(&arg) == true);
}
TEST_CASE("Validation of new string argument") {
    args_parse::Validator validator;
    args_parse::StringArg arg('s', "string");
    REQUIRE(validator.validateNewArgument(&arg) == true);
}
TEST_CASE("Validation of null argument") {
    args_parse::Validator validator;
    args_parse::BoolArg* arg = nullptr;
    REQUIRE_FALSE(validator.validateNewArgument(arg));
}
TEST_CASE("Validation of short name existence") {
    args_parse::Validator validator;
    args_parse::BoolArg arg;
    arg.setShortName('a');
    std::unordered_map<char, args_parse::Arg*> shortNameArgs;
    REQUIRE(validator.validateShortExists(&arg, shortNameArgs));
}
TEST_CASE("Validation of short name not set") {
    args_parse::Validator validator;
    args_parse::BoolArg arg;
    std::unordered_map<char, args_parse::Arg*> shortNameArgs;
    REQUIRE_FALSE(validator.validateShortIsNotSet(&arg));
}
TEST_CASE("Validation of existing short name") {
    args_parse::Validator validator;
    args_parse::BoolArg arg1, arg2;
    arg1.setShortName('a');
    std::unordered_map<char, args_parse::Arg*> shortNameArgs = { {'a',&arg1}};
    arg2.setShortName('a');
    REQUIRE_FALSE(validator.validateShortExists(&arg2, shortNameArgs));
}
TEST_CASE("Validation of long name existence") {
    args_parse::Validator validator;
    args_parse::BoolArg arg;
    arg.setLongName("setLong");
    std::unordered_map<std::string, args_parse::Arg*> longNameArgs;
    REQUIRE(validator.validateLongExists(&arg, longNameArgs));
}
TEST_CASE("Validation of long name not set") {
    args_parse::Validator validator;
    args_parse::BoolArg arg;
    std::unordered_map<std::string, args_parse::Arg*> longNameArgs;
    REQUIRE_FALSE(validator.validateLongIsNotSet(&arg));
}
TEST_CASE("Validation of existing long name") {
    args_parse::Validator validator;
    args_parse::BoolArg arg1, arg2;
    arg1.setLongName("setLong");
    std::unordered_map<std::string, args_parse::Arg*> longNameArgs = { {"setLong",&arg1}};
    arg2.setLongName("setLong");
    REQUIRE_FALSE(validator.validateLongExists(&arg2, longNameArgs));
}
TEST_CASE("Validation of value presence") {
    args_parse::Validator validator;
    std::string value = "testValue";
    REQUIRE(validator.validateValuePresence(value));
}
TEST_CASE("Validation of empty value") {
    args_parse::Validator validator;
    std::string value = "";
    REQUIRE_FALSE(validator.validateValuePresence(value));
}
TEST_CASE("Validation of int range") {
    args_parse::Validator validator;
    std::string value = "12";
    int left = 0;
    int right = 15;
    REQUIRE(validator.validateIntRange(value, left, right));
}
TEST_CASE("Validation of int out of range") {
    args_parse::Validator validator;
    std::string value = "16";
    int left = 0;
    int right = 15;
    REQUIRE_FALSE(validator.validateIntRange(value, left, right));
}
TEST_CASE("Validation of string length") {
    args_parse::Validator validator;
    std::string value = "12345678";
    int maxLength = 9;
    REQUIRE(validator.validateStringLength(value, maxLength));
}
TEST_CASE("Validation of string length out of limit") {
    args_parse::Validator validator;
    std::string value = "123456789";
    int maxLength = 9;
    REQUIRE(validator.validateStringLength(value, maxLength));
}
TEST_CASE("Validation of int not being an int") {
    args_parse::Validator validator;
    std::string value = "t";
    int left = 0;
    int right = 15;
    REQUIRE_FALSE(validator.validateIntRange(value, left, right));
}
TEST_CASE("Parsing short arguments") {
    args_parse::ArgsParser parser;
    args_parse::BoolArg arg('b',"bool");
    parser.add(&arg);

    const char* argv[] = { "args_parse_demo", "-b" };
    int argc = sizeof(argv) / sizeof(argv[0]);

    parser.parse(argc, argv);
    REQUIRE(arg.isDefined());
}
TEST_CASE("Parsing long arguments") {
    args_parse::ArgsParser parser;
    args_parse::BoolArg arg('b', "bool");
    parser.add(&arg);

    const char* argv[] = { "args_parse_demo", "--bool" };
    int argc = sizeof(argv) / sizeof(argv[0]);

    parser.parse(argc, argv);
    REQUIRE(arg.isDefined());
}
TEST_CASE("Parsing short arguments with values") {
    args_parse::ArgsParser parser;
    args_parse::StringArg arg('s', "string");
    parser.add(&arg);

    const char* argv[] = { "args_parse_demo", "-s", "value"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    parser.parse(argc, argv);
    REQUIRE(arg.value() == "value");
}
TEST_CASE("Parsing long arguments with values") {
    args_parse::ArgsParser parser;
    args_parse::StringArg arg('s', "string");
    parser.add(&arg);

    const char* argv[] = { "args_parse_demo", "--string", "value" };
    int argc = sizeof(argv) / sizeof(argv[0]);

    parser.parse(argc, argv);
    REQUIRE(arg.value() == "value");
}
TEST_CASE("Parsing invalid long argument with values") {
    args_parse::ArgsParser parser;
    args_parse::StringArg arg('s', "string");
    parser.add(&arg);

    const char* argv[] = { "args_parse_demo", "--long", "value" };
    int argc = sizeof(argv) / sizeof(argv[0]);

    parser.parse(argc, argv);
    REQUIRE_FALSE(arg.value() == "value");
}
TEST_CASE("Parsing shortened argument with values") {
    args_parse::ArgsParser parser;
    args_parse::StringArg arg('s', "string");
    parser.add(&arg);

    const char* argv[] = { "args_parse_demo", "-str", "value" };
    int argc = sizeof(argv) / sizeof(argv[0]);

    parser.parse(argc, argv);
    REQUIRE(arg.value() == "value");
}




//TEST_CASE("Dummy", "[dummy]")
//{
//    int a = 0;
//    int b = 1;
//
//    REQUIRE(a != b);
//}
//
//TEST_CASE("Section example", "[dummy][section]")
//{
//    int a = 0;
//    int b = 1;
//    std::cout << "before all sections: " << a << ", " << b << std::endl;
//
//    SECTION("The first section") {
//        REQUIRE(a != b);
//
//        a = 10;
//        b = a;
//        std::cout << "inside the first section: " << a << ", " << b << std::endl;
//
//        REQUIRE(a == b);
//    }
//
//    SECTION("The second section") {
//        REQUIRE(a != b);
//
//        b = 0;
//        std::cout << "inside the second section: " << a << ", " << b << std::endl;
//
//        REQUIRE(a == b);
//
//        SECTION("A nested section #1") {
//            std::cout << "on enter into nested #1: " << a << ", " << b << std::endl;
//
//            a = 3;
//            b = 4;
//
//            REQUIRE(a != b);
//        }
//
//        SECTION("A nested section #2") {
//            std::cout << "on enter into nested #2: " << a << ", " << b << std::endl;
//
//            a = 5;
//            b = 6;
//
//            REQUIRE(a != b);
//        }
//
//        SECTION("A nested section #3") {
//            std::cout << "on enter into nested #3: " << a << ", " << b << std::endl;
//
//            a = 7;
//            b = 8;
//
//            REQUIRE(a != b);
//        }
//    }
//}
//
//TEST_CASE("Assertions for negation", "[require_false]")
//{
//    std::unique_ptr<int> d/*{ new int(10) }*/;
//    REQUIRE_FALSE(d);
//}
//
//TEST_CASE("Assertions for exceptions", "[throws]")
//{
//    REQUIRE_NOTHROW([]{}());
//
//    REQUIRE_THROWS(
//        []{ throw std::runtime_error{ "Dummy" }; }());
//
//    REQUIRE_THROWS_AS(
//        []{ throw std::runtime_error{ "Dummy" }; }(),
//        std::runtime_error);
//
//    REQUIRE_THROWS_WITH(
//        []{ throw std::runtime_error{ "Dummy" }; }(),
//        "Dummy");
//
//    REQUIRE_THROWS_WITH(
//        []{ throw std::runtime_error{ "Message with Dummy inside" }; }(),
//        Catch::Matchers::StartsWith("Message") &&
//            Catch::Matchers::ContainsSubstring("Dummy"));
//}
//
//TEST_CASE("Assertions with matchers", "[require_that][matchers]")
//{
//    std::string value{ "This is a dummy value" };
//
//    REQUIRE_THAT(value,
//        Catch::Matchers::StartsWith("This") &&
//        Catch::Matchers::ContainsSubstring("dummy") &&
//        Catch::Matchers::EndsWith("value"));
//}

