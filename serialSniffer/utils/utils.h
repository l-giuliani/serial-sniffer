#ifndef __UTILS_H__
#define __UTILS_H__

#include <string>

namespace NUtils {

class StrFormatter {
public:
    virtual std::string format(const std::string& str) = 0;
};

class HexFormatter : public StrFormatter {
public:
    std::string format(const std::string& str);
};

}


class Utils {

};

#endif