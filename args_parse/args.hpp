#pragma once

#include <string>
#include <unordered_map>
#include <iostream>

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
		}

		// метод для получения значения аргумента
		const T& value() const { return value_; }
		// метод для проверки определенности аргумента
		bool isDefined() const { return defined_; }

	private:
		T value_;
		bool defined_ = false;
	};

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
		bool add(Arg* arg);
		void printHelp() const;
		void parse(int argc, const char** argv);
		void executeArgument(Arg* arg, int argc, const char** argv, int& i);
		void parseShortArgument(char shortArg, int argc, const char** argv, int& i);
		void parseLongArgument(const std::string& longArg, int argc, const char** argv, int& i);
		void parseShortArgumentEquals(char shortName, const std::string& value);
		void parseLongArgumentEquals(const std::string& longName, const std::string& value);
		void executeEquals(Arg* arg, const std::string& value);

	private:
		std::unordered_map<char, Arg*> shortNameArgs_;
		std::unordered_map<std::string, Arg*> longNameArgs_;
	};
} // namespace args_parse