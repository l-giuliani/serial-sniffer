#include <catch2/catch_test_macros.hpp>
#include <cstdlib>
#include <string>

#include "guards.h"
#include "kernelComm.h"
#include "netlink_data.h"

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

    SECTION("Register Callback") {
        KernelComm kc;

        kc.initAndConnect();

        bool res = kc.registerCallback();
        REQUIRE(res == true);

        kc.disconnect();
    }

}