#include "args.hpp"
#include "validator.hpp"
#include <iostream>

namespace args_parse {
	// возвращаем имя короткого аргумента
	char Arg::shortName() const {
		return shortName_;
	}

	// возвращаем имя длинного аргумента
	const std::string& Arg::longName() const {
		return longName_;
	}

	// установка короткого имени аргумента
	void Arg::setShortName(char shortName) {
		shortName_ = shortName;
	}

	// установка длинного имени аргумента
	void Arg::setLongName(const std::string& longName) {
		longName_ = longName;
	}

	// добавление аргумента в парсер
	bool ArgsParser::add(Arg* arg) {
		// проверка: имя у аргумента не пустое
		if (!Validator::validateNewArgument(arg))
			return false;

		// проверка: короткое имя у аргумента не дублирует имена существующих
		if (!Validator::validateShortExists(arg, shortNameArgs_))
			return false;

		// проверка: длинное имя у аргумента не дублирует имена существующих
		if (!Validator::validateLongExists(arg, longNameArgs_))
			return false;

		// если имя не пустое, добавить в список коротких имен
		if (arg->shortName() != '\0') {
			shortNameArgs_[arg->shortName()] = arg;
		}

		// если имя не пустое, добавить в список длинных имен
		if (!arg->longName().empty()) {
			longNameArgs_[arg->longName()] = arg;
		}
		return true;
	}

	// вывести справку
	void ArgsParser::printHelp() const {
		std::cout << "Usage:\t[options]\t[description]" << std::endl;
		for (const auto& pair : longNameArgs_) {
			std::cout << "  -" << pair.second->shortName() << ", --" << pair.first << "    " << pair.second->GetDescription() << std::endl;
		}
	}

	//обработать значения командной строки
	void ArgsParser::parse(int argc, const char** argv) {
		for (int i = 1; i < argc; ++i) {
			std::string arg = argv[i];
			if (arg.size() > 1 && arg[0] == '-') {
				// если аргумент начинается с --, то он длинный
				if (arg[1] == '-') {
					//обрабатываем длинный аргумент
					size_t equalPos = arg.find('=');
					if (equalPos != std::string::npos) {
						// аргумент со знаком равенства
						std::string longName = arg.substr(2, equalPos - 2);
						std::string value = arg.substr(equalPos + 1);
						parseLongArgumentEquals(longName, value);
					}
					else {
						// аргумент без знака равенства
						parseLongArgument(arg.substr(2), argc, argv, i);
					}
				}
				// если аргумент не является длинным, то считаем его коротким или сокращенным
				else {
					size_t equalPos = arg.find('=');
					if (equalPos != std::string::npos) {
						// аргумент со знаком равенства
						char shortName = arg[1];
						std::string value = arg.substr(equalPos + 1);
						parseShortArgumentEquals(shortName, value);
					}
					else if (arg.size() > 2) {
						// аргумент сразу после короткого имени
						char shortName = arg[1];
						std::string value = arg.substr(2);
						parseShortArgumentEquals(shortName, value);
					}
					else {
						// аргумент без значения
						parseShortArgument(arg[1], argc, argv, i);
					}
				}
			}
		}
	}

	// обработать короткие и сокращенные аргументы
	void ArgsParser::parseShortArgument(char shortName, int argc, const char** argv, int& i) {
		auto iter = shortNameArgs_.find(shortName);
		if (iter != shortNameArgs_.end()) {
			executeArgument(iter->second, argc, argv, i);
		}
		else {
			std::cerr << "Error: Unknown argument '-" << shortName << "'" << std::endl;
		}
	}

	// обработать короткие  аргументы со знаком равно
	void ArgsParser::parseShortArgumentEquals(char shortName, const std::string& value) {
		auto iter = shortNameArgs_.find(shortName);
		if (iter != shortNameArgs_.end()) {
			executeEquals(iter->second, value);
		}
		else {
			std::cerr << "Error: Unknown argument '-" << shortName << "'" << std::endl;
		}
	}

	// обработать длинные аргументы
	void ArgsParser::parseLongArgument(const std::string& longName, int argc, const char** argv, int& i) {
		auto iter = longNameArgs_.find(longName);
		if (iter != longNameArgs_.end()) {
			executeArgument(iter->second, argc, argv, i);
		}
		else {
			std::cerr << "Error: Unknown argument '--" << longName << "'" << std::endl;
		}
	}
	// обработать длинные аргументы со знаком равно
	void ArgsParser::parseLongArgumentEquals(const std::string& longName, const std::string& value) {
		auto iter = longNameArgs_.find(longName);
		if (iter != longNameArgs_.end()) {
			executeEquals(iter->second, value);
		}
		else {
			std::cerr << "Error: Unknown argument '--" << longName << "'" << std::endl;
		}
	}

	// добавить значения к аргументам
	void ArgsParser::executeArgument(Arg* arg, int argc, const char** argv, int& i) {
		if (dynamic_cast<MultiArg<int>*>(arg) != nullptr) {
			while (i + 1 < argc) {
				std::string value = argv[i + 1];
				if (value.empty() || value[0] == '-')
					break;
				if (!Validator::validateInt(value)) {
					break;
				}
				arg->setValue(value);
				++i;
			}
		}
		else if (dynamic_cast<MultiArg<std::string>*>(arg) != nullptr) {
			while (i + 1 < argc) {
				std::string value = argv[i + 1];
				if (value.empty() || value[0] == '-')
					break;
				if (!Validator::validateStringLength(value, 50)) {
					break;
				}
				arg->setValue(value);
				++i;
			}
		}
		else if (dynamic_cast<MultiArg<bool>*>(arg) != nullptr) {
			while (i + 1 < argc) {
				std::string value = argv[i + 1];
				if (value.empty() || value[0] == '-')
					break;
				if (value == "true" || value == "false") {
					arg->setValue(value);
				}
				else {
					break;
				}
				arg->setValue(value);
				++i;
			}
		}
		else if (dynamic_cast<MultiArg<float>*>(arg) != nullptr) {
			while (i + 1 < argc) {
				std::string value = argv[i + 1];
				if (value.empty() || value[0] == '-')
					break;
				if (!std::stof(value)) {
					break;
				}
				arg->setValue(value);
				++i;
			}
		}
		// если аргумент типа bool, то не ожидаем от него значения в командной строке
		else if (dynamic_cast<SingleArg<bool>*>(arg) != nullptr) {
			arg->setValue("true");
		}
		// для остальных типов данных ожидаем значение для аргумента
		else {
			if (i + 1 < argc) {
				arg->setValue(argv[++i]);
			}
			else {
				arg->setValue("true");
			}
		}
	}
	void ArgsParser::executeEquals(Arg* arg, const std::string& value) {
		// Если аргумент типа BoolArg, не ожидается значение
		if (dynamic_cast<SingleArg<bool>*>(arg) != nullptr) {
			arg->setValue("true");
		}
		// Для остальных типов данных ожидается значение аргумента
		else {
			arg->setValue(value);
		}
	}
} // namespace args_parse