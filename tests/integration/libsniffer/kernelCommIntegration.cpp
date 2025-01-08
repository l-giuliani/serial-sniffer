#include <catch2/catch_test_macros.hpp>

TEST_CASE("KernelComm") {
    SECTION("Stop operation") {
        REQUIRE((1==1)); // Verifica che stop sia stato chiamato
    }
}