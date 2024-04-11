#pragma once

#include "args.hpp"
#include <string>

namespace args_parse {
	class Validator {
	public:
		static bool validateNewArgument(const Arg* arg);
		static bool validateValuePresence(const std::string& value);

		static bool validateShortIsNotSet(const Arg* arg);
		static bool validateShortExists(const Arg* arg, const std::unordered_map<char, Arg*>& shortNameArgs_);

		static bool validateLongIsNotSet(const Arg* arg);
		static bool validateLongExists(const Arg* arg, const std::unordered_map<std::string, Arg*>& longNameArgs_);

		static bool validateInt(const std::string& value);
		static bool validateFloat(const std::string& value);
		static bool validateStringLength(const std::string& value, size_t maxLength);
	};
}