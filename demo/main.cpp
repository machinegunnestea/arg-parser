#include <args_parse/args.hpp>
#include <iostream>

int main(int argc, const char** argv) {
	args_parse::ArgsParser parser;

	// Define arguments
	args_parse::BoolArg help('h', "help");
	args_parse::IntArg number('g', "getNumber");
	args_parse::IntArg age('a', "age");
	args_parse::StringArg output('o', "output");

	// Add arguments to parser
	parser.add(&help);
	parser.add(&number);
	parser.add(&age);
	parser.add(&output);

	// Parse arguments
	parser.parse(argc, argv);
	parser.printHelp();

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
	if (age.isDefined()) {
		std::cout << "Input d value: " << age.value() << std::endl;
	}
	return 0;
}