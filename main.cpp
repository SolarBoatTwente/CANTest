
#include <iostream>
#include <iomanip>
#include "lib/communication/canbus.h"
#include <libconfig.h++>
#include "lib/communication/CANOpenMaster.h"

int main() {
    std::cout << R"(
   ____     __         ___            __ ______                __
  / __/__  / /__ _____/ _ )___  ___ _/ //_  __/    _____ ___  / /____
 _\ \/ _ \/ / _ `/ __/ _  / _ \/ _ `/ __// / | |/|/ / -_) _ \/ __/ -_)
/___/\___/_/\_,_/_/ /____/\___/\_,_/\__//_/  |__,__/\__/_//_/\__/\__/
)"
    << std::endl;
    Config cfg("../boat.cfg");
    CanBus can_control(cfg.read("can.control"));

    return 0;
}
