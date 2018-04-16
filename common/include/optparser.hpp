#include <map>
#include <string>
#include <vector>

#ifndef __OPTPARSER_hpp__
#define __OPTPARSER_hpp__

namespace our {

class OptParser {

  private:
    std::map<std::string, bool> list_of_options_short;
    std::map<std::string, std::string> name_to_shortname;
    int number_of_options;

    /*
     * function to check whether the supplied string is in the correct option
     * format
     * is_short = 1 if the supplied option is the shortname, and zero otherwise
     */
    bool checkOption(const std::string &option, int is_short = 1);

    /*
     * function to strip the option of the initial hyphens
     * is_short = 1 if the supplied option is the shortname, and zero otherwise
     */
    std::string stripOptionName(const std::string &option, int is_short = 1);

    /*
     * function to add option to list of options
     */
    void addToOptList(const std::string &option_shortname);

  public:
    /*
     * function to add option to list of options and map the full name of the
     * option to the short name (if provided)
     * returns the index of the added option in the list of options
     */
    OptParser() : number_of_options(0) {}

    void addOption(const std::string &option_shortname);
    void addOption(const std::string &option_shortname,
                   const std::string &option_name);

    /*
     * function to parse the command line arguments, marking the valid options
     * as found when they arrive
     */
    void parse(int argc, const char **argv);

    /*
     * function to check whether option is included in the command line
     * arguments or not
     * returns:
     *       1, if option found in command line arguments
     *       0, if option not found in command line arguments
     *      -1, if option not added to the parser
     */
    int checkIncluded(const std::string &option);
};

extern OptParser config;

} // namespace our

#endif
