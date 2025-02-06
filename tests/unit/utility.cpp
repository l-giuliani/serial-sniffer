// #include <catch2/catch_test_macros.hpp>

// #include <thread>
// #include <future>
// #include "c_timer.h"


// class TestAction : public CustomAction {
// private:
//     std::promise<bool>& prom;
// public:
//     TestAction(std::promise<bool>& prom) : prom(prom) {

//     }
//     void action() {
//         prom.set_value(true);
//     }
// };

// TEST_CASE("Timer", "[Utility]") {

//     std::promise<bool> prom;
//     std::future<bool> fut = prom.get_future();
//     TestAction ta(prom);

//     CustomTimer ct(ta, 200);
//     ct.start();
//     std::future_status cv_stat = fut.wait_for(std::chrono::milliseconds(300));
//     REQUIRE(cv_stat == std::future_status::ready);
//     bool res = fut.get();
//     REQUIRE(res == true);
    
//     std::promise<bool> prom2;
//     std::future<bool> fut2 = prom2.get_future();
//     TestAction ta2(prom2);
//     CustomTimer ct2(ta2, 800);
//     ct2.start();
//     cv_stat = fut2.wait_for(std::chrono::milliseconds(100));
//     REQUIRE(cv_stat != std::future_status::ready);
//     ct2.stop();
//     cv_stat = fut2.wait_for(std::chrono::milliseconds(300));
//     REQUIRE(cv_stat != std::future_status::ready);
// }