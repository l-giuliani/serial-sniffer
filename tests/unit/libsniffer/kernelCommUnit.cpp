// #include <catch2/catch_test_macros.hpp>

// #include "kernelComm.h"
// #include "netlink_data.h"
// #include <vector>
// #include <variant>
// #include <iostream>

// class TestSubscriber : public KernelCommSubscriber {
// private:
// public:
//     void onData(const KernelMulticastData& kmd) {}
// };

// TEST_CASE("Subscribe", "[KernelComm]") {
//     KernelComm kc;

//     std::vector<KernelCommSubscriber*>& subscribers = kc.getSubscribers();
//     REQUIRE(subscribers.size() == 0);
    
//     TestSubscriber ts;
//     kc.subscribe(&ts);

//     REQUIRE(subscribers.size() == 1);
//     REQUIRE(subscribers.at(0) == &ts);
// }

// TEST_CASE("DataToSend", "[KernelComm]") { 
//     DataToSend data;
//     data.reset();
//     int j = 0;
//     while(std::shared_ptr<DataDto> d = data.next()) {
//         j++;
//     }
//     REQUIRE(j == 0);

//     std::string tty1("ttyS1");
//     std::string tty2("ttyS2");

//     std::vector<std::string> sv;
//     sv.push_back(tty1);
//     sv.push_back(tty2);

//     data.add(SNIFF_ATTR_SER_DEVICE, tty1);
//     data.add(SNIFF_ATTR_SER_DEVICE, tty2);
//     data.reset();
//     int i = 0;
//     while(std::shared_ptr<DataDto> d = data.next()) {
//         REQUIRE(d->getAttribute() == SNIFF_ATTR_SER_DEVICE);
//         REQUIRE(std::holds_alternative<std::string>(d->getData()));
//         REQUIRE(sv.at(i).compare(std::get<std::string>(d->getData())) == 0);

//         i++;
//     }
// }
