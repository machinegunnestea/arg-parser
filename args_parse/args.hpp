#pragma once

#include "validator.hpp"
#include <string>
#include <unordered_map>

namespace args_parse {
	class Arg {
	public:
		Arg() : shortName_('\0') {}

		void setShortName(char shortName);
		void setLongName(const std::string& longName);

		char shortName() const;
		const std::string& longName() const;

		virtual void setValue(const std::string& value) = 0;

		virtual const Validator* getValidator() const = 0;

	private:
		char shortName_;
		std::string longName_;
	protected:
		ValidatorShortName shortNameValidator_;
	};

	class BoolArg : public Arg {
	public:

		BoolArg(char shortName, const std::string& longName);
		void setValue(const std::string& value) override;
		bool isDefined() const;

	private:
		bool value_;
	};

	class StringArg : public Arg {
	public:

		StringArg(char shortName, const std::string& longName);
		void setValue(const std::string& value) override;
		const std::string& value() const;
		const Validator* getValidator() const override;

	private:
		std::string value_;
		ValidatorShortName validator_;
	};
	class IntArg : public Arg {
	public:
		IntArg(char shortName, const std::string& longName);
		void setValue(const std::string& value) override;
		int value() const;
		bool isDefined() const;
		const Validator* getValidator() const override;

	private:
		int value_;
		ValidatorShortName validator_;
	};

	class ArgsParser {
	public:
		bool add(Arg* arg);
		void printHelp() const;
		void parse(int argc, const char** argv);

	private:
		std::unordered_map<char, Arg*> shortNameArgs_;
		std::unordered_map<std::string, Arg*> longNameArgs_;
	};
} // namespace args_parse