#include <algorithm>
#include <iostream>
#include <optparser.hpp>
#include <string>

namespace our {

bool OptParser::checkOption(const std::string option, int is_short) {
    int n = option.length();
    if (is_short) { /* if shortname is provided */
        if (n <= 1)
            return false;
        if (n > 2)
            return false;
        if (option[0] != '-')
            return false;
    } else { /* if fullname is provided */
        if (n <= 3)
            return false;
        if (option[0] != '-' || option[1] != '-')
            return false;
    }
    return true;
}

std::string OptParser::stripOptionName(const std::string option, int is_short) {
    if (is_short)
        return std::string(1, option[1]);
    else
        return option.substr(2);
}

void OptParser::addToOptList(const std::string option_shortname) {
    auto iter = list_of_options_short.find(option_shortname);
    if (iter == list_of_options_short.end()) {
        number_of_options += 1;
        list_of_options_short[option_shortname] = false;
    }
}

void OptParser::addOption(const std::string option_shortname) {
    if (checkOption(option_shortname))
        addToOptList(stripOptionName(option_shortname));
    else
        std::cerr << option_shortname
                  << " cannot be added as it is not of a valid format.\n";
}

void OptParser::addOption(const std::string option_shortname,
                          const std::string option_name) {
    if (checkOption(option_shortname) && checkOption(option_name, 0)) {
        addToOptList(stripOptionName(option_shortname));
        name_to_shortname[stripOptionName(option_name)] =
            stripOptionName(option_shortname);
    } else {
        std::cerr << "(" << option_shortname << ", " << option_name
                  << ") cannot be addes as it is not of a valid format.\n";
    }
}

void OptParser::parse(int argc, const char **argv) {
    for (int i = 0; i < argc; i++) {
        std::string current_option(current_option);
        std::string shortname;
        if (checkOption(current_option)) {
            shortname = stripOptionName(current_option);
            if (list_of_options_short.find(shortname) ==
                list_of_options_short.end()) {
                std::cerr << shortname << " is not a valid option.\n";
                return;
            }
        } else if (checkOption(current_option, 0)) {
            std::string name = stripOptionName(current_option, 0);
            if (name_to_shortname.find(name) == name_to_shortname.end()) {
                std::cerr << name << " is not a valid option.\n";
                return;
            }
            shortname = name_to_shortname[name];
        } else {
            std::cerr
                << current_option
                << " is provided in a wrong format, hence cannot be parsed.";
            return;
        }
        list_of_options_short[shortname] = true;
    }
}

int OptParser::checkIncluded(std::string option) {
    int n = option.length();
    if (n == 1) {
        if (list_of_options_short.find(option) == list_of_options_short.end()) {
            std::cerr << option
                      << " has not been added to the list of options\n";
            return -1;
        } else {
            return list_of_options_short[option];
        }
    } else {
        if (name_to_shortname.find(option) == name_to_shortname.end()) {
            std::cerr << option
                      << " has not been added to the list of options\n";
        } else {
            return list_of_options_short[name_to_shortname[option]];
        }
    }
}

} // namespace our