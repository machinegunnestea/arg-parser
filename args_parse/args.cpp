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
					std::string longName = arg.substr(2);
					auto iter = longNameArgs_.find(longName);
					if (iter != longNameArgs_.end()) {
						if (iter->second->shortName() == 'h') {
							iter->second->setValue("true");
						}
						else {
							if (i + 1 < argc) {
								iter->second->setValue(argv[++i]);
							}
							else {
								iter->second->setValue("true");
							}
						}
					}
					else {
						std::cerr << "Error: Unknown argument '--" << longName << "'" << std::endl;
					}
				}
				else {
					char shortName = arg[1];
					auto iter = shortNameArgs_.find(shortName);
					if (iter != shortNameArgs_.end()) {
						if (iter->second->shortName() == 'h') {
							iter->second->setValue("true");
						}
						else {
							if (i + 1 < argc) {
								iter->second->setValue(argv[++i]);
							}
							else {
								iter->second->setValue("true");
							}
						}
					}
					else {
						std::cerr << "Error: Unknown argument '-" << shortName << "'" << std::endl;
					}
				}
			}
		}
	}
} // namespace args_parse