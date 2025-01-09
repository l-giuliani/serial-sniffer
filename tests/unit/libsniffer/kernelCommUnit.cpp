#include <catch2/catch_test_macros.hpp>

#include "kernelComm.h"
#include <vector>

class TestSubscriber : public KernelCommSubscriber {
private:
public:
    void onData(const KernelMulticastData& kmd) {}
};

TEST_CASE("Subscribe", "[KernelComm]") {
    KernelComm kc;

    std::vector<KernelCommSubscriber*>& subscribers = kc.getSubscribers();
    REQUIRE(subscribers.size() == 0);
    
    TestSubscriber ts;
    kc.subscribe(&ts);

    REQUIRE(subscribers.size() == 1);
    REQUIRE(subscribers.at(0) == &ts);
}