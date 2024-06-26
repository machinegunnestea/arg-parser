﻿#pragma once

#include <string>
#include <unordered_map>
#include <iostream>
#include <chrono>
#include <sstream>

namespace args_parse {
	/// @brief Класс для представления аргументов командной строки
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
		virtual void setValue(const std::string_view& value) = 0;

		//методы для установки и получения описания аргумента
		std::string GetDescription() const { return description_; }
		void SetDescription(const std::string& description) { description_ = description; }

	private:
		char shortName_;
		std::string longName_;
		std::string description_;
	};

	/// @brief пользовательский класс для подсчета времени
	class UserChrono {
		std::chrono::microseconds m_;
	public:
		UserChrono() = default;
		UserChrono(std::chrono::microseconds m_) : m_{ m_ } {}
		std::chrono::microseconds GetMicroseconds() const { return m_; }

		// шаблон функции для парсинга пользовательского времени из строки [number][measure]
		bool ParseUserChrono(UserChrono& userChrono, const std::string& operand);
	};

	/// @brief ожидает операнд в виде [число][единица измерения], например 12s, 12d, 12m
	inline bool ParseUserChrono(UserChrono& userChrono, const std::string_view& operand) {
		//std::stringstream ss{ operand.data() };
		if (operand.size() < 2) // Ensure operand has at least two characters
			return false;
		// string_view error message
		long long value;
		char type = operand.back();
		std::string valueStr = std::string(operand).substr(0, operand.size() - 1);
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

	/// @brief Шаблон класса для аргумента с единственным значением
	template<typename T>
	class SingleArg : public Arg {
	public:
		SingleArg(char shortName, const std::string& longName) : Arg(shortName, longName) {}
		SingleArg(const std::string& longName) : Arg(longName) {}
		SingleArg() {}

		// ref to template function
		// метод для установки значения аргумента
		void setValue(const std::string_view& value) override
		{
			std::cerr << "Error: Unsupported type for SingleArg setValue: " << value << std::endl;
		}

		// метод для получения значения аргумента
		const T& value() const { return value_; }
		// метод для проверки определенности аргумента
		bool isDefined() const { return defined_; }

	private:
		T value_;
		bool defined_ = false;
	};

	/// @brief Шаблон класса для аргумента с множественным значением
	template<typename T>
	class MultiArg : public Arg {
	public:
		MultiArg(char shortName, const std::string& longName) : Arg(shortName, std::move(longName)) {}
		MultiArg(const std::string& longName) : Arg(std::move(longName)) {}
		MultiArg() {}

		//ref to template
		// метод для установки значения аргумента
		void setValue(const std::string_view& value) override
		{
			std::cerr << "Error: Unsupported type for SingleArg setValue: " << value << std::endl;
		}

		// метод для получения значения аргумента
		const std::vector<T>& values() const { return values_; }
		// метод для проверки определенности аргумента
		bool isDefined() const { return !values_.empty(); }

	private:
		std::vector<T> values_;
	};

	/// @brief Класс для парсинга аргументов командной строки
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
		void parseLongArgument(const std::string_view& longArg, int argc, const char** argv, int& i);
		// вспомогательный метод для parse для обработки короткого названия аргумента со знаком равно
		void parseShortArgumentEquals(char shortName, const std::string_view& value);
		// вспомогательный метод для parse для обработки длинного названия аргумента со знаком равно
		void parseLongArgumentEquals(const std::string_view& longName, const std::string_view& value);
		// вспомогательный метод для parse для добавление значений к аргументам со знаком равно
		void executeEquals(Arg* arg, const std::string_view& value);

	private:
		std::unordered_map<char, Arg*> shortNameArgs_;
		std::unordered_map<std::string_view, Arg*> longNameArgs_;
	};
	// Специализация шаблонов метода setValue для различных типов данных
	template<>
	inline void SingleArg<int>::setValue(const std::string_view& value) {
		try {
			value_ = std::stoi(std::string(value));
			defined_ = true;
		}
		catch (...) {
			std::cerr << "Error: Invalid integer value: " << value << std::endl;
		}
	}

	template<>
	inline void SingleArg<float>::setValue(const std::string_view& value) {
		try {
			value_ = std::stof(std::string(value));
			defined_ = true;
		}
		catch (...) {
			std::cerr << "Error: Invalid float value: " << value << std::endl;
		}
	}

	template<>
	inline void SingleArg<bool>::setValue(const std::string_view& value) {
		if (std::string(value) == "true" || std::string(value) == "1")
		{
			value_ = true;
			defined_ = true;
		}

		else if (std::string(value) == "false" || std::string(value) == "0")
		{
			value_ = false;
			defined_ = true;
		}
		else
			std::cerr << "Error: Invalid boolean value: " << value << std::endl;
	}

	template<>
	inline void SingleArg<std::string>::setValue(const std::string_view& value) {
		value_ = value;
		defined_ = true;
	}

	template<>
	inline void SingleArg<UserChrono>::setValue(const std::string_view& value) {
		UserChrono tempChrono;
		if (ParseUserChrono(tempChrono, value)) {
			value_ = tempChrono;
			defined_ = true;
		}
		else {
			std::cerr << "Error: Invalid UserChrono value: " << value << std::endl;
		}
	}

	template<>
	inline void MultiArg<int>::setValue(const std::string_view& value) {
		try {
			values_.push_back(std::stoi(std::string(value)));
		}
		catch (...) {
			std::cerr << "Error: Invalid int value: " << value << std::endl;
		}
	}

	template<>
	inline void MultiArg<float>::setValue(const std::string_view& value) {
		try {
			values_.push_back(std::stof(std::string(value)));
		}
		catch (...) {
			std::cerr << "Error: Invalid float value: " << value << std::endl;
		}
	}

	template<>
	inline void MultiArg<bool>::setValue(const std::string_view& value) {
		if (value == "true" || value == "1")
			values_.push_back(true);
		else if (value == "false" || value == "0")
			values_.push_back(false);
		else
			std::cerr << "Error: Invalid boolean value: " << value << std::endl;
	}

	template<>
	inline void MultiArg<std::string>::setValue(const std::string_view& value) {
		values_.push_back(std::string(value));
	}
} // namespace args_parse