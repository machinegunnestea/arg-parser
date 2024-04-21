#include "args.hpp"
#include "validator.hpp"
#include <iostream>
#include <unordered_map>
#include <string>

namespace args_parse {
	/// @brief проверка: аргумент не нулевой
	bool Validator::validateNewArgument(const Arg* arg) {
		if (!arg) {
			std::cerr << "Error:Attempted to add a null argument." << std::endl;
			return false;
		}
		return true;
	}

	/// @brief проверка: короткое имя не установлено
	bool Validator::validateShortIsNotSet(const Arg* arg) {
		if (arg->shortName() == '\0') {
			std::cerr << "Error: Short name is not set for argument." << std::endl;
			return false;
		}
		return true;
	}

	/// @brief проверка: короткое имя уже существует
	bool Validator::validateShortExists(const Arg* arg, const std::unordered_map<char, Arg*>& shortNameArgs_) {
		if (shortNameArgs_.find(arg->shortName()) != shortNameArgs_.end()) {
			std::cerr << "Error: Short name '" << arg->shortName() << "' already exists." << std::endl;
			return false;
		}
		return true;
	}

	/// @brief проверка: длинное имя не установлено
	bool Validator::validateLongIsNotSet(const Arg* arg) {
		if (arg->longName().empty()) {
			std::cerr << "Error: Long name is not set for argument." << std::endl;
			return false;
		}
		return true;
	}

	/// @brief проверка: длинное имя уже существует
	bool Validator::validateLongExists(const Arg* arg, const std::unordered_map<std::string_view, Arg*>& longNameArgs_) {
		if (longNameArgs_.find(arg->longName()) != longNameArgs_.end()) {
			std::cerr << "Error: Long name '" << arg->longName() << "' already exists." << std::endl;
			return false;
		}
		return true;
	}

	/// @brief проверка: у аргумента существует значение
	bool Validator::validateValuePresence(const std::string& value) {
		if (value.empty()) {
			std::cerr << "Error: No value provided for argument." << std::endl;
			return false;
		}
		return true;
	}

	/// @brief проверка: у аргумента существует значение, оно целочисленное
	bool Validator::validateInt(const std::string& value) {
		// проверка целочисленности
		try {
			std::ignore = std::stoi(value);
		}
		catch (const std::exception& e) {
			std::cerr << "Error: Invalid integer value:" << e.what() << std::endl;
			return false;
		}
		return true;
	}
	/// @brief проверка: у аргумента существует значение, оно c плавающей запятой
	bool Validator::validateFloat(const std::string& value) {
		// проверка целочисленности
		try {
			std::ignore = std::stof(value);
			//(void)  std::stof(value); // стилизация си
		}
		catch (const std::exception& e) {
			std::cerr << "Error: Invalid float value:" << e.what() << std::endl;
			return false;
		}
		return true;
	}
	/// @brief проверка: у аргумента существует значение, оно строковое и его длина не превышает установленного максимума
	bool Validator::validateStringLength(const std::string& value, size_t maxLength) {
		if (value.length() > maxLength) {
			std::cerr << "Error: String value is longer than expected (" << maxLength << " characters max)." << std::endl;
			return false;
		}
		return true;
	}
}