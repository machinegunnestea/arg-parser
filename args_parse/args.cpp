#include "args.hpp"
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
		if (shortNameValidator_.validate(std::string(1, shortName()))) {
			value_ = value;
		}
		else {
			std::cerr << "Error: No short name provided for an argument." << std::endl;
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
		if (validator_.validate(value)) {
			value_ = std::stoi(value);
		}
		else {
			std::cerr << "Error: Invalid value for integer argument: " << std::endl;
		}
		
	}
	int IntArg::value() const {
		return value_;
	}
	const Validator* IntArg::getValidator() const {
		return &validator_;
	}
	bool IntArg::isDefined() const {
		return value_;
	}

	bool ArgsParser::add(Arg* arg) {
		if (!arg) {
			std::cerr << "Error: Attempted to add a null argument." << std::endl;
			return false;
		}

		if (arg->shortName() != '\0') {
			if (shortNameArgs_.find(arg->shortName()) != shortNameArgs_.end()) {
				std::cerr << "Error: Short name '" << arg->shortName() << "' already exists." << std::endl;
				return false;
			}
			shortNameArgs_[arg->shortName()] = arg;
		}

		if (!arg->longName().empty()) {
			if (longNameArgs_.find(arg->longName()) != longNameArgs_.end()) {
				std::cerr << "Error: Long name '" << arg->longName() << "' already exists." << std::endl;
				return false;
			}
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