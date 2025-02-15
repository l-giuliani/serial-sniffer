#ifndef __UTILS_H__
#define __UTILS_H__

#include <string>

namespace NUtils {

/**
 * @class StrFormatter
 * @brief formatter interface
 * 
 * log can be formatter and a concreate formatter can be used
 * in order to perform this action
*/
class StrFormatter {
public:
    virtual std::string format(const std::string& str) = 0;
};

/**
 * @class HexFormatter
 * @brief concrete formatter
 * 
 * perform hex formatting of log
*/
class HexFormatter : public StrFormatter {
public:
    std::string format(const std::string& str);
};

}


class Utils {

};

#endif