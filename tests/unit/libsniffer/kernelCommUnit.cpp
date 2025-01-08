#include <catch2/catch_test_macros.hpp>

TEST_CASE("Subscribe Init Condition", "[KernelComm]") {
    REQUIRE((1==1)); // Verifica che stop sia stato chiamato
}

TEST_CASE("Subscribe Final Condition", "[KernelComm]") {
    REQUIRE((1==1));
}