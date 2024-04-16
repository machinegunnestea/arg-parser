#include <args_parse/args.hpp>
#include <iostream>
#include <chrono>

int main(int argc, const char** argv) {
	args_parse::ArgsParser parser;

	// Define arguments
	args_parse::SingleArg<bool> help('h', "help");
	args_parse::SingleArg<int> number('g', "getNumber");
	args_parse::SingleArg<std::string> output('o', "output");
	args_parse::SingleArg<float> flo('f', "float");
	args_parse::SingleArg<args_parse::UserChrono> us('u', "user");

	help.SetDescription("single bool argument shows the list of the arguments");
	number.SetDescription("single int argument shows Input g value [int]");
	output.SetDescription("single string argument shows Output o value [string]");
	flo.SetDescription("single float argument shows Input f value [float]");
	us.SetDescription("user single user argument shows time converted to microseconds [value][measure]");

	args_parse::MultiArg<int> multiInt('a', "age");
	args_parse::MultiArg<std::string> multiString('s', "str");
	args_parse::MultiArg<bool> multiBool('b', "bool");
	args_parse::MultiArg<float> multiFlo('m', "mflo");

	multiBool.SetDescription("multiple bool argument shows Input multi bool values: [bool] [bool] [bool]...");
	multiInt.SetDescription("multiple int argument shows Input g value [int] [int] [int]...");
	multiString.SetDescription("multiple string argument shows Output o value [string] [string] [string]...");
	multiFlo.SetDescription("multiple float argument shows Input f value [float] [float] [float]...");

	// Add arguments to parser
	parser.add(&help);
	parser.add(&number);
	parser.add(&output);
	parser.add(&flo);
	parser.add(&us);


	parser.add(&multiInt);
	parser.add(&multiString);
	parser.add(&multiBool);
	parser.add(&multiFlo);

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
	// if getNumber was activated
	if (us.isDefined()) {
		// Get the value of the time argument
		args_parse::UserChrono timeValue = us.value();
		// Do something with the time value
		std::cout << "Time argument value: " << timeValue.GetMicroseconds().count() << " microseconds" << std::endl;
	}
	else {
		std::cout << "Time argument not provided." << std::endl;
	}

	// if float was activated
	if (flo.isDefined()) {
		std::cout << "Input f value: " << number.value() << std::endl;
	}
	if (multiInt.isDefined()) {
		std::cout << "Input multi int values: ";
		for (const auto& val : multiInt.values()) {
			std::cout << val << " ";
		}
		std::cout << std::endl;
	}
	if (multiFlo.isDefined()) {
		std::cout << "Input multi float values: ";
		for (const auto& val : multiFlo.values()) {
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