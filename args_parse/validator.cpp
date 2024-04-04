#include "args.hpp"
#include "validator.hpp"
#include <iostream>
#include <unordered_map>
#include <string>

namespace args_parse {
	// проверка: аргумент не нулевой
	bool Validator::validateNewArgument(const Arg* arg) {
		if (!arg) {
			std::cerr << "Error:Attempted to add a null argument." << std::endl;
			return false;
		}
		return true;
	}

	// проверка: короткое имя не установлено
	bool Validator::validateShortIsNotSet(const Arg* arg) {
		if (arg->shortName() == '\0') {
			std::cerr << "Error: Short name is not set for argument." << std::endl;
			return false;
		}
		return true;
	}

	// проверка: короткое имя уже существует
	bool Validator::validateShortExists(const Arg* arg, const std::unordered_map<char, Arg*>& shortNameArgs_) {
		if (shortNameArgs_.find(arg->shortName()) != shortNameArgs_.end()) {
			std::cerr << "Error: Short name '" << arg->shortName() << "' already exists." << std::endl;
			return false;
		}
		return true;
	}

	// проверка: длинное имя не установлено
	bool Validator::validateLongIsNotSet(const Arg* arg) {
		if (arg->longName().empty()) {
			std::cerr << "Error: Long name is not set for argument." << std::endl;
			return false;
		}
		return true;
	}

	// проверка: длинное имя уже существует
	bool Validator::validateLongExists(const Arg* arg, const std::unordered_map<std::string, Arg*>& longNameArgs_) {
		if (longNameArgs_.find(arg->longName()) != longNameArgs_.end()) {
			std::cerr << "Error: Long name '" << arg->longName() << "' already exists." << std::endl;
			return false;
		}
		return true;
	}

	// проверка: у аргумента существует значение
	bool Validator::validateValuePresence(const std::string& value) {
		if (value.empty()) {
			std::cerr << "Error: No value provided for argument." << std::endl;
			return false;
		}
		return true;
	}

	// проверка: у аргумента существует значение, оно целочисленное и находится в заданном диапазоне
	bool Validator::validateIntRange(const std::string& value, int leftBorder, int rightBorder) {
		int intValue;
		// проверка целочисленности
		try {
			intValue = std::stoi(value);
		}
		catch (const std::exception& e) {
			std::cerr << "Error: Invalid integer value:" << e.what() << std::endl;
			return false;
		}
		// проверка принадлежности диапазону
		if (intValue < leftBorder || intValue > rightBorder) {
			std::cerr << "Error: Integer value out of range ("
				<< leftBorder
				<< ";"
				<< rightBorder
				<< ")."
				<< std::endl;
			return false;
		}
		return true;
	}

	// проверка: у аргумента существует значение, оно строковое и его длина не превышает установленного максимума
	bool Validator::validateStringLength(const std::string& value, size_t maxLength) {
		if (value.length() > maxLength) {
			std::cerr << "Error: String value is longer than expected (" << maxLength << " characters max)." << std::endl;
			return false;
		}
		return true;
	}
}