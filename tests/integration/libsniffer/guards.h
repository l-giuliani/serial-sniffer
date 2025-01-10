#ifndef __GUARDS_H__
#define __GUARDS_H__

#include <cstdlib>
#include <string>

class KernelModuleGuard {
public:
    KernelModuleGuard(const std::string& loadScript, const std::string& unloadScript)
        : unloadScript_(unloadScript), loaded_(false) {
        int ret = std::system(loadScript.c_str());
        if (ret == 0) {
            loaded_ = true;
        } else {
            throw std::runtime_error("Failed to load kernel module");
        }
    }

    ~KernelModuleGuard() {
        if (loaded_) {
            std::system(unloadScript_.c_str());
        }
    }

private:
    std::string unloadScript_;
    bool loaded_;
};

#endif