#include "args.hpp"
#include "validator.hpp"
#include <iostream>

namespace args_parse {
	// установка короткого имени аргумента
	void Arg::setShortName(char shortName) {
		shortName_ = shortName;
	}

	// установка длинного имени аргумента
	void Arg::setLongName(const std::string& longName) {
		longName_ = longName;
	}

	// возвращаем имя короткого аргумента
	char Arg::shortName() const {
		return shortName_;
	}

	// возвращаем имя длинного аргумента
	const std::string& Arg::longName() const {
		return longName_;
	}
	// конструктор с параметрами для типа bool
	BoolArg::BoolArg(char shortName, const std::string& longName) : value_(false) {
		setShortName(shortName);
		setLongName(longName);
	}

	// конструктор без параметров для типа bool
	BoolArg::BoolArg() : value_(false) {}

	// установка значения для bool
	void BoolArg::setValue(const std::string& value) {
		if (value != "false") {
			value_ = true;
		}
	}

	// проверка установлено ли значение для bool
	bool BoolArg::isDefined() const {
		return value_;
	}

	// конструктор с параметрами для типа string
	StringArg::StringArg(char shortName, const std::string& longName) {
		setShortName(shortName);
		setLongName(longName);
	}

	// конструктор без параметров для типа string
	StringArg::StringArg() {}

	// установка значения для string
	// метод ожидает значение для аргумента и максимально допустимую длину
	void StringArg::setValue(const std::string& value) {
		int maxLength = 25;
		if (Validator::validateValuePresence(value) && Validator::validateStringLength(value, maxLength)) {
			value_ = value;
		}
	}

	// возвращаем значение аргумента string
	const std::string& StringArg::value() const {
		return value_;
	}

	// конструктор с параметрами для типа int
	IntArg::IntArg(char shortName, const std::string& longName) {
		setShortName(shortName);
		setLongName(longName);
	}

	// конструктор без параметров для типа int
	IntArg::IntArg() {}

	// установка значения для int
	// метод ожидает значение для аргумента, левую и правую границы допустимых значений
	void IntArg::setValue(const std::string& value) {
		int left = -127;
		int right = 128;
		if (Validator::validateValuePresence(value) && Validator::validateIntRange(value, left, right)) {
			value_ = std::stoi(value);
		}
	}
	// возвращаем значение аргумента int
	int IntArg::value() const {
		return value_;
	}

	// проверка установлено ли значение для int
	bool IntArg::isDefined() const {
		return value_;
	}

	// добавление аргументав парсер
	bool ArgsParser::add(Arg* arg) {
		// проверка: имя у аргумента не пустое
		if (!Validator::validateNewArgument(arg))
			return false;

		//if (!Validator::validateShortIsNotSet(arg))
		//	return false;
		//if (!Validator::validateLongIsNotSet(arg))
		//	return false;

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
		std::cout << "Usage:\t[options]" << std::endl;
		for (const auto& pair : longNameArgs_) {
			std::cout << "  -" << pair.second->shortName() << ", --" << pair.first << std::endl;
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
					parseLongArgument(arg.substr(2), argc, argv, i);
				}
				// если аргумент не является длинным, то считаем его коротким или сокращенным
				else {
					//обрабатываем короткий или сокращенный аргумент
					parseShortArgument(arg[1], argc, argv, i);
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

	// добавить значения к аргументам
	void ArgsParser::executeArgument(Arg* arg, int argc, const char** argv, int& i) {
		// если аргумент типа bool, то не ожидаем от него значения в командной строке
		if (dynamic_cast<BoolArg*>(arg) != nullptr) {
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
} // namespace args_parse