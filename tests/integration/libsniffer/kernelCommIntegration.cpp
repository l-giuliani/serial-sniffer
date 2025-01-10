#include <catch2/catch_test_macros.hpp>
#include <cstdlib>
#include <string>

#include "kernelComm.h"
#include "netlink_data.h"

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

TEST_CASE("KernelComm") {

    KernelModuleGuard guard("./scripts/loadSerialBridge.sh", "./scripts/unloadSerialBridge.sh");

    SECTION("Init and Connect and Disconnect") {
        KernelComm kc;

        bool res = kc.initAndConnect();
        REQUIRE(res == true);
        REQUIRE(kc.getConnected() == true);

        res = kc.disconnect();
        REQUIRE(res == true);
        REQUIRE(kc.getConnected() == false);
    }

    SECTION("Register To Multicast Group") {
        KernelComm kc;

        kc.initAndConnect();

        int res = kc.registerToMulticastGroup("errorFamily","errorGroup");
        INFO("res = " << res);
        REQUIRE(res < 0);
        res = kc.registerToMulticastGroup(GENL_FAMILY_NAME,"errorGroup");
        REQUIRE(res < 0);
        res = kc.registerToMulticastGroup(GENL_FAMILY_NAME, GENL_MULTICAST_GROUP);
        REQUIRE(res >= 0);
        bool res2 = kc.removeFromMulticastGroup(GENL_FAMILY_NAME, GENL_MULTICAST_GROUP);
        REQUIRE(res2 == true);

        kc.disconnect();
    }

}