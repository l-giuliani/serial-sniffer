#include <csignal>
#include <cstdlib>
#include "dto.h"
#include "systemServices.h"
#include "services.h"

void sigIntHandler(int signal) {
    NServices::Services::sniffStop();
    std::exit(signal);
}

int main(int argc, char* argv[]) {
    NDto::SystemDto systemDto;

    std::signal(SIGINT, sigIntHandler);

    NServices::SystemServices::initSystem();
    NServices::SystemServices::parseParams(systemDto, argc, argv);

   NServices::Services::sniffStart(systemDto);
}