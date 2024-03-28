#include <args_parse/args.hpp>
#include <iostream>

int main(int argc, const char** argv) {
	args_parse::ArgsParser parser;

	// Define arguments
	args_parse::BoolArg help('h', "help");
	args_parse::IntArg number('g', "getNumber");
	args_parse::StringArg output('o', "output");

	// Add arguments to parser
	parser.add(&help);
	parser.add(&number);
	parser.add(&output);

	// Parse arguments
	parser.parse(argc, argv);

	// if help was activated
	if (help.isDefined()) {
		parser.printHelp();
	}
	// if output was activated
	if (!output.value().empty()) {
		std::cout << "Output value: " << output.value() << std::endl;
	}
	// if getNumber was activated
	if (number.value() != 0) {
		std::cout << "Input value: " << number.value() << std::endl;
	}
	return 0;
}