#include <catch2/catch_test_macros.hpp>

#include "guards.h"
#include "kernelComm.h"
#include "netlink_data.h"

TEST_CASE("Helpers") {

    KernelModuleGuard guard("./scripts/loadSerialBridge.sh", "./scripts/unloadSerialBridge.sh");

    SECTION("Init and Connect and Disconnect") {
        
    }

}