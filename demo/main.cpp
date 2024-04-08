#include <args_parse/args.hpp>
#include <iostream>

int main(int argc, const char** argv) {
	args_parse::ArgsParser parser;

	// Define arguments
	args_parse::BoolArg help('h', "help");
	args_parse::IntArg number('g', "getNumber");
	args_parse::StringArg output('o', "output");

	args_parse::MultiInt multiInt('a', "age");
	args_parse::MultiString multiString('s', "str");
	args_parse::MultiBool multiBool('b', "bool");

	// Add arguments to parser
	parser.add(&help);
	parser.add(&number);
	parser.add(&output);

	parser.add(&multiInt);
	parser.add(&multiString);
	parser.add(&multiBool);

	// Parse arguments
	parser.parse(argc, argv);
	//parser.printHelp();

	//if help was activated
	if (help.isDefined()) {
		parser.printHelp();
	}
	//if output was activated
	if (!output.value().empty()) {
		std::cout << "Output value: " << output.value() << std::endl;
	}
	// if getNumber was activated
	if (number.isDefined()) {
		std::cout << "Input g value: " << number.value() << std::endl;
	}
	if (multiInt.isDefined()) {
		std::cout << "Input multi int values: ";
		for (const auto& val : multiInt.values()) {
			std::cout << val << " ";
		}
		std::cout << std::endl;
	}

	if (multiString.isDefined()) {
		std::cout << "Input multi string values: ";
		for (const auto& val : multiString.values()) {
			std::cout << val << " ";
		}
		std::cout << std::endl;
	}
	if (multiBool.isDefined()) {
		std::cout << "Input multi bool values: ";
		for (const auto& val : multiBool.values()) {
			std::cout << val << " ";
		}
		std::cout << std::endl;
	}

	return 0;
}