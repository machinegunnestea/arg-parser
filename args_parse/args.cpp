#include "args.hpp"
#include "validator.hpp"
#include <iostream>

namespace args_parse {
	void Arg::setShortName(char shortName) {
		shortName_ = shortName;
	}

	void Arg::setLongName(const std::string& longName) {
		longName_ = longName;
	}

	char Arg::shortName() const {
		return shortName_;
	}

	const std::string& Arg::longName() const {
		return longName_;
	}

	BoolArg::BoolArg(char shortName, const std::string& longName) : value_(false) {
		setShortName(shortName);
		setLongName(longName);
	}
	BoolArg::BoolArg() : value_(false) {}
	void BoolArg::setValue(const std::string& value) {
		if (value != "false") {
			value_ = true;
		}
	}

	bool BoolArg::isDefined() const {
		return value_;
	}

	StringArg::StringArg(char shortName, const std::string& longName) {
		setShortName(shortName);
		setLongName(longName);
	}
	StringArg::StringArg() {}

	void StringArg::setValue(const std::string& value) {
		int maxLength = 25;
		if (Validator::validateValuePresence(value) && Validator::validateStringLength(value, maxLength)) {
			value_ = value;
		}
	}

	const std::string& StringArg::value() const {
		return value_;
	}

	IntArg::IntArg(char shortName, const std::string& longName) {
		setShortName(shortName);
		setLongName(longName);
	}
	IntArg::IntArg(){}

	void IntArg::setValue(const std::string& value) {
		int left = -127;
		int right = 128;
		if (Validator::validateValuePresence(value) && Validator::validateIntRange(value, left, right)) {
			value_ = std::stoi(value);
		}
	}
	int IntArg::value() const {
		return value_;
	}

	bool IntArg::isDefined() const {
		return value_;
	}

	bool ArgsParser::add(Arg* arg) {
		if (!Validator::validateNewArgument(arg))
			return false;

		//if (!Validator::validateShortIsNotSet(arg))
		//	return false;
		//if (!Validator::validateLongIsNotSet(arg))
		//	return false;

		if (!Validator::validateShortExists(arg, shortNameArgs_))
			return false;
		if (!Validator::validateLongExists(arg, longNameArgs_))
			return false;

		if (arg->shortName() != '\0') {
			shortNameArgs_[arg->shortName()] = arg;
		}

		if (!arg->longName().empty()) {
			longNameArgs_[arg->longName()] = arg;
		}
		return true;
	}

	void ArgsParser::printHelp() const {
		std::cout << "Usage:\t[options]" << std::endl;
		for (const auto& pair : longNameArgs_) {
			std::cout << "  -" << pair.second->shortName() << ", --" << pair.first << std::endl;
		}
	}

	void ArgsParser::parse(int argc, const char** argv) {
		for (int i = 1; i < argc; ++i) {
			std::string arg = argv[i];
			if (arg.size() > 1 && arg[0] == '-') {
				if (arg[1] == '-') {
					parseLongArgument(arg.substr(2), argc, argv, i);
				}
				else {
					parseShortArgument(arg[1], argc, argv, i);
				}
			}
		}
	}
	void ArgsParser::parseShortArgument(char shortName, int argc, const char** argv, int& i) {
		auto iter = shortNameArgs_.find(shortName);
		if (iter != shortNameArgs_.end()) {
			executeArgument(iter->second, argc, argv, i);
		}
		else {
			std::cerr << "Error: Unknown argument '-" << shortName << "'" << std::endl;
		}
	}
	void ArgsParser::parseLongArgument(const std::string& longName, int argc, const char** argv, int& i) {
		auto iter = longNameArgs_.find(longName);
		if (iter != longNameArgs_.end()) {
			executeArgument(iter->second, argc, argv, i);
		}
		else {
			std::cerr << "Error: Unknown argument '--" << longName << "'" << std::endl;
		}
	}
	void ArgsParser::executeArgument(Arg* arg, int argc, const char** argv, int& i) {
		if (dynamic_cast<BoolArg*>(arg) != nullptr) {
			arg->setValue("true");
		}
		else {
			if (i + 1 < argc) {
				arg->setValue(argv[++i]);
			}
			else {
				arg->setValue("true");
			}
		}
	}

} // namespace args_parse