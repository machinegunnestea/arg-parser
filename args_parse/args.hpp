#pragma once

#include <string>
#include <unordered_map>
#include <iostream>
#include <chrono>
#include <sstream>

namespace args_parse {
	class Arg {
	public:
		// конструктор для короткого и длинного имени аргумента
		Arg(char shortName, const std::string& longName) : shortName_(shortName), longName_(longName) {}
		// конструктор для длинного имени аргумента
		Arg(const std::string& longName) : shortName_('\0'), longName_(longName) {}
		// без параметров
		Arg() : shortName_('\0') {}

		//методы для установки и получения короткого и длинного имени аргумента
		void setShortName(char shortName);
		void setLongName(const std::string& longName);
		char shortName() const;
		const std::string& longName() const;

		//виртуальный метод для установки значения аргумента
		virtual void setValue(const std::string& value) = 0;

		//методы для установки и получения описания аргумента
		std::string GetDescription() const { return description_; }
		void SetDescription(const std::string& description) { description_ = description; }

	private:
		char shortName_;
		std::string longName_;
		std::string description_;
	};

	// пользовательский класс для подсчета времени
	class UserChrono {
		std::chrono::microseconds m_;
	public:
		UserChrono() = default;
		UserChrono(std::chrono::microseconds m_) : m_{ m_ } {}
		std::chrono::microseconds GetMicroseconds() const { return m_; }

		// шаблон функции для парсинга пользовательского времени из строки [number][measure]
		template<typename T>
		friend std::enable_if_t<std::is_same_v<T, UserChrono>, bool>
			ParseUserChrono(UserChrono& userChrono, const std::string& operand);
	};

	// ожидает операнд в виде [число][единица измерения], например 12s, 12d, 12m
	// конвертирует в микросекунды
	template<typename T>
	std::enable_if_t<std::is_same_v<T, UserChrono>, bool>
		ParseUserChrono(UserChrono& userChrono, const std::string& operand) {
		//std::stringstream ss{ operand.data() };
		if (operand.size() < 2) // Ensure operand has at least two characters
			return false;

		long long value;
		char type = operand.back();
		std::string valueStr = operand.substr(0, operand.size() - 1);
		std::stringstream ss{ valueStr.data() };

		ss >> value;

		if (!ss || ss.rdbuf()->in_avail() != 0)
			return false;

		std::chrono::microseconds user;

		switch (type) {
		case 'd':
			user = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::hours(value * 24));
			break;
		case 'h':
			user = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::hours(value));
			break;
		case 's':
			user = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::seconds(value));
			break;
		case 'm':
			user = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::milliseconds(value));
			break;
		case 'n':
			user = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::nanoseconds(value));
			break;
		default:
			return false;
		}

		userChrono = UserChrono{ user };

		return true;
	}

	// Шаблон класса для аргумента с единственным значением
	template<typename T>
	class SingleArg : public Arg {
	public:
		SingleArg(char shortName, const std::string& longName) : Arg(shortName, longName) {}
		SingleArg(const std::string& longName) : Arg(longName) {}
		SingleArg() {}

		// метод для установки значения аргумента
		void setValue(const std::string& value) override {
			if constexpr (std::is_same_v<T, std::string>) {
				value_ = value;
				defined_ = true;
			}
			else if constexpr (std::is_same_v<T, int>) {
				try {
					value_ = std::stoi(value);
					defined_ = true;
				}
				catch (...) {
					std::cerr << "Error: Invalid integer value: " << value << std::endl;
				}
			}
			else if constexpr (std::is_same_v<T, float>) {
				try {
					value_ = std::stof(value);
					defined_ = true;
				}
				catch (...) {
					std::cerr << "Error: Invalid float value: " << value << std::endl;
				}
			}
			else if constexpr (std::is_same_v<T, bool>) {
				// Handle boolean value (e.g., 'true', 'false')
				if (value == "true" || value == "1")
					value_ = true;
				else if (value == "false" || value == "0")
					value_ = false;
				else
					std::cerr << "Error: Invalid boolean value: " << value << std::endl;
				defined_ = true;
			}
			if constexpr (std::is_same_v<T, UserChrono>) {
				if (!ParseUserChrono<T>(value_, value)) {
					std::cerr << "Error: Invalid UserChrono value: " << value << std::endl;
				}
				defined_ = true;
			}
		}

		// метод для получения значения аргумента
		const T& value() const { return value_; }
		// метод для проверки определенности аргумента
		bool isDefined() const { return defined_; }

	private:
		T value_;
		bool defined_ = false;
	};

	// Шаблон класса для аргумента с множественным значением
	template<typename T>
	class MultiArg : public Arg {
	public:
		MultiArg(char shortName, const std::string& longName) : Arg(shortName, longName) {}
		MultiArg(const std::string& longName) : Arg(longName) {}
		MultiArg() {}

		// метод для установки значения аргумента
		void setValue(const std::string& value) override {
			if constexpr (std::is_same_v<T, std::string>) {
				values_.push_back(value);
			}
			else if constexpr (std::is_same_v<T, int>) {
				try {
					values_.push_back(std::stoi(value));
				}
				catch (...) {
					std::cerr << "Error: Invalid integer value: " << value << std::endl;
				}
			}
			else if constexpr (std::is_same_v<T, float>) {
				try {
					values_.push_back(std::stof(value));
				}
				catch (...) {
					std::cerr << "Error: Invalid float value: " << value << std::endl;
				}
			}
			else if constexpr (std::is_same_v<T, bool>) {
				// Handle boolean value (e.g., 'true', 'false')
				if (value == "true" || value == "1")
					values_.push_back(true);
				else if (value == "false" || value == "0")
					values_.push_back(false);
				else
					std::cerr << "Error: Invalid boolean value: " << value << std::endl;
			}
		}

		// метод для получения значения аргумента
		const std::vector<T>& values() const { return values_; }
		// метод для проверки определенности аргумента
		bool isDefined() const { return !values_.empty(); }

	private:
		std::vector<T> values_;
	};

	class ArgsParser {
	public:
		// добавление аргумента
		bool add(Arg* arg);
		//вывод справки о доступных аргументах
		void printHelp() const;
		// обработка командной строки
		void parse(int argc, const char** argv);
		// вспомогательный метод для parse для добавление значений к аргументам
		void executeArgument(Arg* arg, int argc, const char** argv, int& i);
		// вспомогательный метод для parse для обработки короткого названия аргумента
		void parseShortArgument(char shortArg, int argc, const char** argv, int& i);
		// вспомогательный метод для parse для обработки длинного названия аргумента
		void parseLongArgument(const std::string& longArg, int argc, const char** argv, int& i);
		// вспомогательный метод для parse для обработки короткого названия аргумента со знаком равно
		void parseShortArgumentEquals(char shortName, const std::string& value);
		// вспомогательный метод для parse для обработки длинного названия аргумента со знаком равно
		void parseLongArgumentEquals(const std::string& longName, const std::string& value);
		// вспомогательный метод для parse для добавление значений к аргументам со знаком равно
		void executeEquals(Arg* arg, const std::string& value);

	private:
		std::unordered_map<char, Arg*> shortNameArgs_;
		std::unordered_map<std::string, Arg*> longNameArgs_;
	};
} // namespace args_parse