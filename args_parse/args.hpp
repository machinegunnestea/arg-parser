#pragma once

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

	private:
		char shortName_;
		std::string longName_;
	};

	class BoolArg : public Arg {
	public:
		BoolArg();
		BoolArg(char shortName, const std::string& longName);
		void setValue(const std::string& value) override;
		bool isDefined() const;

	private:
		bool value_;
	};

	class StringArg : public Arg {
	public:
		StringArg();
		StringArg(char shortName, const std::string& longName);
		void setValue(const std::string& value) override;
		const std::string& value() const;

	private:
		std::string value_;
	};
	class IntArg : public Arg {
	public:
		IntArg();
		IntArg(char shortName, const std::string& longName);
		void setValue(const std::string& value) override;
		int value() const;
		bool isDefined() const;

	private:
		int value_;
	};

	class MultiInt : public Arg {
	public:
		MultiInt(char shortName, const std::string& longName);
		MultiInt();
		void setValue(const std::string& value) override;
		const std::vector<int>& values() const;
		bool isDefined() const;
	private:
		std::vector<int> values_;
	};

	class MultiBool : public Arg {
	public:
		MultiBool(char shortName, const std::string& longName);
		MultiBool();
		void setValue(const std::string& value) override;
		const std::vector<bool>& values() const;
		bool isDefined() const;
	private:
		std::vector<bool> values_;
	};

	class MultiString : public Arg {
	public:
		MultiString(char shortName, const std::string& longName);
		MultiString();
		void setValue(const std::string& value) override;
		const std::vector<std::string>& values() const;
		bool isDefined() const;
	private:
		std::vector<std::string> values_;
	};

	class ArgsParser {
	public:
		bool add(Arg* arg);
		void printHelp() const;
		void parse(int argc, const char** argv);
		void executeArgument(Arg* arg, int argc, const char** argv, int& i);
		void parseShortArgument(char shortArg, int argc, const char** argv, int& i);
		void parseLongArgument(const std::string& longArg, int argc, const char** argv, int& i);

	private:
		std::unordered_map<char, Arg*> shortNameArgs_;
		std::unordered_map<std::string, Arg*> longNameArgs_;
	};
} // namespace args_parse