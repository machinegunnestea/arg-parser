#include <catch2/catch_all.hpp>
#include <args_parse/args.hpp>
#include <args_parse/validator.hpp>
#include <iostream>
#include <memory>
#include <unordered_map>

TEST_CASE("Validation of arguments", "[args_validation]") {
	args_parse::Validator validator;

	SECTION("Validation of new bool argument") {
		args_parse::BoolArg arg('b', "bool");
		REQUIRE(validator.validateNewArgument(&arg));
	}
	SECTION("Validation of new int argument") {
		args_parse::IntArg arg('i', "int");
		REQUIRE(validator.validateNewArgument(&arg));
	}
	SECTION("Validation of new string argument") {
		args_parse::StringArg arg('s', "string");
		REQUIRE(validator.validateNewArgument(&arg));
	}
	SECTION("Validation of null argument") {
		args_parse::BoolArg* arg = nullptr;
		REQUIRE_FALSE(validator.validateNewArgument(arg));
	}
	SECTION("Validation of short name not set") {
		args_parse::BoolArg arg;
		std::unordered_map<char, args_parse::Arg*> shortNameArgs;
		REQUIRE_FALSE(validator.validateShortIsNotSet(&arg));
	}
	SECTION("Validation of long name not set") {
		args_parse::BoolArg arg;
		std::unordered_map<std::string, args_parse::Arg*> longNameArgs;
		REQUIRE_FALSE(validator.validateLongIsNotSet(&arg));
	}
}

TEST_CASE("Validation of duplicate arguments", "[args_dup_validation]") {
	args_parse::Validator validator;

	SECTION("Validation of short name existence") {
		args_parse::BoolArg arg;
		arg.setShortName('a');
		std::unordered_map<char, args_parse::Arg*> shortNameArgs;
		REQUIRE(validator.validateShortExists(&arg, shortNameArgs));
	}
	SECTION("Validation of long name existence") {
		args_parse::BoolArg arg;
		arg.setLongName("setLong");
		std::unordered_map<std::string, args_parse::Arg*> longNameArgs;
		REQUIRE(validator.validateLongExists(&arg, longNameArgs));
	}
	SECTION("Validation of existing short name") {
		args_parse::BoolArg arg1, arg2;
		arg1.setShortName('a');
		std::unordered_map<char, args_parse::Arg*> shortNameArgs = { {'a',&arg1} };
		arg2.setShortName('a');
		REQUIRE_FALSE(validator.validateShortExists(&arg2, shortNameArgs));
	}
	SECTION("Validation of existing long name") {
		args_parse::BoolArg arg1, arg2;
		arg1.setLongName("setLong");
		std::unordered_map<std::string, args_parse::Arg*> longNameArgs = { {"setLong",&arg1} };
		arg2.setLongName("setLong");
		REQUIRE_FALSE(validator.validateLongExists(&arg2, longNameArgs));
	}
}

TEST_CASE("Validation of argument values", "[args_value_validation]") {
	args_parse::Validator validator;

	SECTION("Validation of value presence") {
		std::string value = "testValue";
		REQUIRE(validator.validateValuePresence(value));
	}
	SECTION("Validation of empty value") {
		std::string value = "";
		REQUIRE_FALSE(validator.validateValuePresence(value));
	}
	SECTION("Validation of int range") {
		std::string value = "12";
		int left = 0;
		int right = 15;
		REQUIRE(validator.validateIntRange(value, left, right));
	}
	SECTION("Validation of int out of range") {
		std::string value = "16";
		int left = 0;
		int right = 15;
		REQUIRE_FALSE(validator.validateIntRange(value, left, right));
	}
	SECTION("Validation of string length") {
		std::string value = "12345678";
		int maxLength = 9;
		REQUIRE(validator.validateStringLength(value, maxLength));
	}
	SECTION("Validation of string length out of limit") {
		std::string value = "123456789";
		int maxLength = 9;
		REQUIRE(validator.validateStringLength(value, maxLength));
	}
	SECTION("Validation of int not being an int") {
		std::string value = "t";
		int left = 0;
		int right = 15;
		REQUIRE_FALSE(validator.validateIntRange(value, left, right));
	}
}

TEST_CASE("Parsing of arguments", "[args_parsing]") {
	args_parse::ArgsParser parser;

	SECTION("Parsing short arguments") {
		args_parse::BoolArg arg('b', "bool");
		parser.add(&arg);

		const char* argv[] = { "args_parse_demo", "-b" };
		const int argc = std::size(argv);

		parser.parse(argc, argv);
		REQUIRE(arg.isDefined());
	}
	SECTION("Parsing long arguments") {
		args_parse::BoolArg arg('b', "bool");
		parser.add(&arg);

		const char* argv[] = { "args_parse_demo", "--bool" };
		const int argc = std::size(argv);

		parser.parse(argc, argv);
		REQUIRE(arg.isDefined());
	}
	SECTION("Parsing short arguments with values") {
		args_parse::StringArg arg('s', "string");
		parser.add(&arg);

		const char* argv[] = { "args_parse_demo", "-s", "value" };
		const int argc = std::size(argv);

		parser.parse(argc, argv);
		REQUIRE(arg.value() == "value");
	}
	SECTION("Parsing long arguments with values") {
		args_parse::StringArg arg('s', "string");
		parser.add(&arg);

		const char* argv[] = { "args_parse_demo", "--string", "value" };
		const int argc = std::size(argv);

		parser.parse(argc, argv);
		REQUIRE(arg.value() == "value");
	}
	SECTION("Parsing invalid long argument with values") {
		args_parse::StringArg arg('s', "string");
		parser.add(&arg);

		const char* argv[] = { "args_parse_demo", "--long", "value" };
		const int argc = std::size(argv);

		parser.parse(argc, argv);
		REQUIRE_FALSE(arg.value() == "value");
	}
	SECTION("Parsing shortened argument with values") {
		args_parse::StringArg arg('s', "string");
		parser.add(&arg);

		const char* argv[] = { "args_parse_demo", "-str", "value" };
		const int argc = std::size(argv);

		parser.parse(argc, argv);
		REQUIRE(arg.value() == "value");
	}
}
TEST_CASE("Validation of multi arguments", "[multi_args]") {
	args_parse::Validator validator;

	SECTION("Validation of MultiInt argument values within range") {
		args_parse::MultiInt arg('m', "multi");
		REQUIRE(validator.validateIntRange("5", -50, 50));
	}
	SECTION("Validation of MultiInt argument values out of range") {
		args_parse::MultiInt arg('m', "multi");
		REQUIRE_FALSE(validator.validateIntRange("60", -50, 50));
	}
	SECTION("Validation of MultiInt argument values are not int") {
		args_parse::MultiInt arg('m', "multi");
		REQUIRE_FALSE(validator.validateIntRange("str", -50, 50));
	}
	SECTION("Validation of MultiBool argument values") {
		args_parse::MultiBool arg('m', "multi");
		REQUIRE(validator.validateValuePresence("true, false"));
	}
	SECTION("Validation of MultiString argument values") {
		args_parse::MultiString arg('m', "multi");
		REQUIRE(validator.validateValuePresence("value"));
	}
}
TEST_CASE("Parsing multi arguments", "[multi_parse]") {
	args_parse::ArgsParser parser;

	SECTION("Parsing of MultiInt arguments") {
		args_parse::MultiInt arg('m', "multi");
		parser.add(&arg);

		const char* argv[] = { "program", "-m", "10", "20", "30" };
		const int argc = std::size(argv);
		//int argc = sizeof(argv) / sizeof(argv[0]);

		parser.parse(argc, argv);

		const auto& values = arg.values();
		REQUIRE(values.size() == 3);
		REQUIRE(values[0] == 10);
		REQUIRE(values[1] == 20);
		REQUIRE(values[2] == 30);
	}
	SECTION("Parsing of MultiBool arguments") {
		args_parse::MultiBool arg('m', "multi");
		parser.add(&arg);

		const char* argv[] = { "program", "-m", "true", "false", "true", "false" };
		const int argc = std::size(argv);
		//int argc = sizeof(argv) / sizeof(argv[0]);

		parser.parse(argc, argv);

		const auto& values = arg.values();
		REQUIRE(values.size() == 4);
		REQUIRE(values[0] == true);
		REQUIRE(values[1] == false);
		REQUIRE(values[2] == true);
		REQUIRE(values[3] == false);
	}
	SECTION("Parsing of MultiString arguments") {
		args_parse::MultiString arg('m', "multi");
		parser.add(&arg);

		const char* argv[] = { "program", "-m", "value1", "value2", "value3", "-m", "value4" };
		const int argc = std::size(argv);

		parser.parse(argc, argv);

		const auto& values = arg.values();
		REQUIRE(values.size() == 4);
		REQUIRE(values[0] == "value1");
		REQUIRE(values[1] == "value2");
		REQUIRE(values[2] == "value3");
		REQUIRE(values[3] == "value4");
	}
}