#pragma once
#include <string>

namespace args_parse {
	class Validator{
	public:
		virtual bool validate(const std::string& value) const = 0;
	};
    class ValidatorInt : public Validator {
    public:
        bool validate(const std::string& value) const override {
            try {
                std::stoi(value);
                return true;
            }
            catch (const std::exception&) {
                return false;
            }
        }
    };
    class ValidatorString : public Validator {
    public:
        bool validate(const std::string& value) const override {
            if (value.length() <= 20)
                return true;
            else
                return false;
        }
    };
    class ValidatorShortName : public Validator {
    public:
        bool validate(const std::string& value) const override {
            char shortName = value[0];
            return shortName != '\0';
        }
    };
}