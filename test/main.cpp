#include <catch2/catch_all.hpp>
#include <args_parse/args.hpp>
#include <args_parse/validator.hpp>
#include <iostream>
#include <memory>
#include <unordered_map>

TEST_CASE("Validation of arguments", "[args_validation]") {
	args_parse::Validator validator;

	SECTION("Validation of new bool argument") {
		args_parse::SingleArg<bool> arg('b', "bool");
		REQUIRE(validator.validateNewArgument(&arg));
	}
	SECTION("Validation of new int argument") {
		args_parse::SingleArg<int> arg('i', "int");
		REQUIRE(validator.validateNewArgument(&arg));
	}
	SECTION("Validation of new string argument") {
		args_parse::SingleArg<std::string> arg('s', "string");
		REQUIRE(validator.validateNewArgument(&arg));
	}
	SECTION("Validation of null argument") {
		args_parse::SingleArg<bool>* arg = nullptr;
		REQUIRE_FALSE(validator.validateNewArgument(arg));
	}
	SECTION("Validation of short name not set") {
		args_parse::SingleArg<bool> arg;
		std::unordered_map<char, args_parse::Arg*> shortNameArgs;
		REQUIRE_FALSE(validator.validateShortIsNotSet(&arg));
	}
	SECTION("Validation of long name not set") {
		args_parse::SingleArg<bool> arg;
		std::unordered_map<std::string, args_parse::Arg*> longNameArgs;
		REQUIRE_FALSE(validator.validateLongIsNotSet(&arg));
	}
}

TEST_CASE("Validation of duplicate arguments", "[args_dup_validation]") {
	args_parse::Validator validator;

	SECTION("Validation of short name existence") {
		args_parse::SingleArg<bool> arg;
		arg.setShortName('a');
		std::unordered_map<char, args_parse::Arg*> shortNameArgs;
		REQUIRE(validator.validateShortExists(&arg, shortNameArgs));
	}
	SECTION("Validation of long name existence") {
		args_parse::SingleArg<bool> arg;
		arg.setLongName("setLong");
		std::unordered_map<std::string, args_parse::Arg*> longNameArgs;
		REQUIRE(validator.validateLongExists(&arg, longNameArgs));
	}
	SECTION("Validation of existing short name") {
		args_parse::SingleArg<bool> arg1, arg2;
		arg1.setShortName('a');
		std::unordered_map<char, args_parse::Arg*> shortNameArgs = { {'a',&arg1} };
		arg2.setShortName('a');
		REQUIRE_FALSE(validator.validateShortExists(&arg2, shortNameArgs));
	}
	SECTION("Validation of existing long name") {
		args_parse::SingleArg<bool> arg1, arg2;
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
	SECTION("Validation of int") {
		std::string value = "12";
		REQUIRE(validator.validateInt(value));
	}
	SECTION("Validation of float") {
		std::string value = "16.2";
		REQUIRE(validator.validateFloat(value));
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
		REQUIRE_FALSE(validator.validateInt(value));
	}
	SECTION("Validation of float not being an int") {
		std::string value = "t";
		REQUIRE_FALSE(validator.validateFloat(value));
	}
}

TEST_CASE("Parsing of arguments", "[args_parsing]") {
	args_parse::ArgsParser parser;

	SECTION("Parsing short arguments") {
		args_parse::SingleArg<bool> arg('b', "bool");
		parser.add(&arg);

		const char* argv[] = { "args_parse_demo", "-b" };
		const int argc = static_cast<int>(std::size(argv));

		parser.parse(argc, argv);
		REQUIRE(arg.isDefined());
	}
	SECTION("Parsing long arguments") {
		args_parse::SingleArg<bool> arg('b', "bool");
		parser.add(&arg);

		const char* argv[] = { "args_parse_demo", "--bool" };
		const int argc = static_cast<int>(std::size(argv));

		parser.parse(argc, argv);
		REQUIRE(arg.isDefined());
	}
	SECTION("Parsing short arguments with values") {
		args_parse::SingleArg<std::string> arg('s', "string");
		parser.add(&arg);

		const char* argv[] = { "args_parse_demo", "-s", "value" };
		const int argc = static_cast<int>(std::size(argv));

		parser.parse(argc, argv);
		REQUIRE(arg.value() == "value");
	}
	SECTION("Parsing long arguments with values") {
		args_parse::SingleArg<std::string> arg('s', "string");
		parser.add(&arg);

		const char* argv[] = { "args_parse_demo", "--string", "value" };
		const int argc = static_cast<int>(std::size(argv));

		parser.parse(argc, argv);
		REQUIRE(arg.value() == "value");
	}
	SECTION("Parsing invalid long argument with values") {
		args_parse::SingleArg<std::string> arg('s', "string");
		parser.add(&arg);

		const char* argv[] = { "args_parse_demo", "--long", "value" };
		const int argc = static_cast<int>(std::size(argv));

		parser.parse(argc, argv);
		REQUIRE_FALSE(arg.value() == "value");
	}
	SECTION("Parsing shortened argument without space or equal sing") {
		args_parse::SingleArg<std::string> arg('s', "string");
		parser.add(&arg);

		const char* argv[] = { "args_parse_demo", "-str" };
		const int argc = static_cast<int>(std::size(argv));

		parser.parse(argc, argv);
		REQUIRE(arg.value() == "tr");
	}
}
TEST_CASE("Validation of multi arguments", "[multi_args]") {
	args_parse::Validator validator;

	SECTION("Validation of MultiInt argument values within range") {
		args_parse::MultiArg<int> arg('m', "multi");
		REQUIRE(validator.validateInt("5"));
	}
	SECTION("Validation of MultiInt argument values are not int") {
		args_parse::MultiArg<int> arg('m', "multi");
		REQUIRE_FALSE(validator.validateInt("str"));
	}
	SECTION("Validation of MultiFloat argument values are not int") {
		args_parse::MultiArg<float> arg('m', "multi");
		REQUIRE_FALSE(validator.validateFloat("str"));
	}
	SECTION("Validation of MultiBool argument values") {
		args_parse::MultiArg<bool> arg('m', "multi");
		REQUIRE(validator.validateValuePresence("true, false"));
	}
	SECTION("Validation of MultiString argument values") {
		args_parse::MultiArg<std::string> arg('m', "multi");
		REQUIRE(validator.validateValuePresence("value"));
	}
}
TEST_CASE("Parsing multi arguments", "[multi_parse]") {
	args_parse::ArgsParser parser;

	SECTION("Parsing of MultiInt arguments") {
		args_parse::MultiArg<int> arg('m', "multi");
		parser.add(&arg);

		const char* argv[] = { "args_parse_demo", "-m", "10", "20", "30" };
		const int argc = static_cast<int>(std::size(argv));

		parser.parse(argc, argv);

		const auto& values = arg.values();
		REQUIRE(values.size() == 3);
		REQUIRE(values[0] == 10);
		REQUIRE(values[1] == 20);
		REQUIRE(values[2] == 30);
	}
	SECTION("Parsing of MultiBool arguments") {
		args_parse::MultiArg<bool> arg('m', "multi");
		parser.add(&arg);

		const char* argv[] = { "args_parse_demo", "-m", "true", "false", "true", "false" };
		const int argc = static_cast<int>(std::size(argv));

		parser.parse(argc, argv);

		const auto& values = arg.values();
		REQUIRE(values.size() == 4);
		REQUIRE(values[0] == true);
		REQUIRE(values[1] == false);
		REQUIRE(values[2] == true);
		REQUIRE(values[3] == false);
	}
	SECTION("Parsing of MultiString arguments") {
		args_parse::MultiArg<std::string> arg('m', "multi");
		parser.add(&arg);

		const char* argv[] = { "args_parse_demo", "-m", "value1", "value2", "value3" };
		const int argc = static_cast<int>(std::size(argv));

		parser.parse(argc, argv);

		const auto& values = arg.values();
		REQUIRE(values.size() == 3);
		REQUIRE(values[0] == "value1");
		REQUIRE(values[1] == "value2");
		REQUIRE(values[2] == "value3");
	}
}