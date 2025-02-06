#include <catch2/catch_test_macros.hpp>

#include <future>
#include <functional>
#include <cstdint> 
#include <chrono>
#include <thread>
#include <iostream>

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
        AsyncSerialSniffer as("/dev/ttyS4");

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
        
        res = kc.initAndConnect();
        REQUIRE(res == true);
        int rs = kc.sendData(GENL_FAMILY_NAME, SNIFF_CMD_SEND_TEST, nullptr);
        REQUIRE(rs >= 0);
        kc.disconnect();

        std::future_status cv_stat = cbFut.wait_for(std::chrono::milliseconds(10000));
        REQUIRE(cv_stat == std::future_status::ready);
        CallbackData cd = cbFut.get();

        INFO("len = " << cd.len);
        INFO("buffer = " << cd.buffer);
        REQUIRE(cd.len > 0);
        REQUIRE(cd.buffer != NULL);
        std::string data((char*)cd.buffer, cd.len);
        INFO("data = " << data);
        REQUIRE(data.compare("CmD_TesT") == 0);

        as.stop();
        as.uninit();
    }

    // SECTION("Keep Alive") {
    //     std::string serialPort = "/dev/ttyTest";
    //     AsyncSerialSniffer as(serialPort);

    //     as.setTestMode(true);
        

    //     std::promise<CallbackData> cbProm;
    //     std::future<CallbackData> cbFut = cbProm.get_future();

    //     std::function<void(uint8_t*, int)> callback = [&cbProm] (uint8_t* buffer, int len) {
    //         CallbackData cd;
    //         cd.buffer = buffer;
    //         cd.len = len;
    //         cbProm.set_value(cd);
    //     };

    //     as.init(callback);
    //     as.start();

    //     std::future_status cv_stat = cbFut.wait_for(std::chrono::milliseconds(10000));
    //     REQUIRE(cv_stat == std::future_status::ready);
    //     CallbackData cb = cbFut.get();

    //     std::string str((char*)cb.buffer, cb.len);
    //     REQUIRE(str.compare(serialPort) == 0);

    //     as.stop();
    //     as.uninit();
    // }

}