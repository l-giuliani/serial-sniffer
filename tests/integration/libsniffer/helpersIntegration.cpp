#include <catch2/catch_test_macros.hpp>

#include <future>
#include <functional>
#include <cstdint> 

#include "guards.h"
#include "kernelComm.h"
#include "helper.h"
#include "netlink_data.h"

typedef struct {
    uint8_t* buffer;
    int len;
} CallbackData;

TEST_CASE("Helpers") {

    KernelModuleGuard guard("./scripts/loadSerialBridge.sh", "./scripts/unloadSerialBridge.sh");

    SECTION("Receive Data from Netlink Group") {
        KernelComm kc;
        AsyncSniffer as;

        std::promise<CallbackData> cbProm;
        std::future<CallbackData> cbFut = cbProm.get_future();

        std::function<void(uint8_t*, int)> callback = [&cbProm] (uint8_t* buffer, int len) {
            CallbackData cd;
            cd.buffer = buffer;
            cd.len = len;
            cbProm.set_value(cd);
        };

        bool res = as.init(callback);
        REQUIRE(res == true);
        as.start();

        kc.initAndConnect();
        res = kc.registerToMulticastGroup(GENL_FAMILY_NAME, GENL_MULTICAST_GROUP);
        REQUIRE(res == true);

        kc.disconnect();

        CallbackData cd = cbFut.get();
    }

}