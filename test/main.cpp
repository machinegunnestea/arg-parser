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
		args_parse::Validator validator;
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
		int argc = sizeof(argv) / sizeof(argv[0]);

		parser.parse(argc, argv);
		REQUIRE(arg.isDefined());
	}
	SECTION("Parsing long arguments") {
		args_parse::BoolArg arg('b', "bool");
		parser.add(&arg);

		const char* argv[] = { "args_parse_demo", "--bool" };
		int argc = sizeof(argv) / sizeof(argv[0]);

		parser.parse(argc, argv);
		REQUIRE(arg.isDefined());
	}
	SECTION("Parsing short arguments with values") {
		args_parse::StringArg arg('s', "string");
		parser.add(&arg);

		const char* argv[] = { "args_parse_demo", "-s", "value" };
		int argc = sizeof(argv) / sizeof(argv[0]);

		parser.parse(argc, argv);
		REQUIRE(arg.value() == "value");
	}
	SECTION("Parsing long arguments with values") {
		args_parse::StringArg arg('s', "string");
		parser.add(&arg);

		const char* argv[] = { "args_parse_demo", "--string", "value" };
		int argc = sizeof(argv) / sizeof(argv[0]);

		parser.parse(argc, argv);
		REQUIRE(arg.value() == "value");
	}
	SECTION("Parsing invalid long argument with values") {
		args_parse::StringArg arg('s', "string");
		parser.add(&arg);

		const char* argv[] = { "args_parse_demo", "--long", "value" };
		int argc = sizeof(argv) / sizeof(argv[0]);

		parser.parse(argc, argv);
		REQUIRE_FALSE(arg.value() == "value");
	}
	SECTION("Parsing shortened argument with values") {
		args_parse::StringArg arg('s', "string");
		parser.add(&arg);

		const char* argv[] = { "args_parse_demo", "-str", "value" };
		int argc = sizeof(argv) / sizeof(argv[0]);

		parser.parse(argc, argv);
		REQUIRE(arg.value() == "value");
	}
}