
#include <iostream>
#include <iomanip>
#include "../lib/communication/canbus.h"
#include <libconfig.h++>
#include "../lib/configuration/Config.h"
#include "../lib/configuration/ConfigSection.h"
#include "../lib/devices/Sonar.h"

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

    //class impl
    class Sonar sonar_front(*new ConfigSection(cfg, "sonar_front"), can_control);

    // create function
    io::CanRouterReadFrame read_sonar_f_distance(sonar_front.message_id_distance, io::CanFlag::NONE, [&](const can_msg* msg, std::error_code ec) {
        sonar_front.doStuffForSonar(msg, ec, read_sonar_f_distance);
        return;
    });

    // set in can
    can_control.registerReadFrame(read_sonar_f_distance);
    can_control.start();
    std::cout << "registered frame and started can" <<std::endl;

    // sending message over the CAN bus
    can_msg msg{};
    msg.len = 8;
    msg.flags = static_cast<uint_least8_t>(io::CanFlag::NONE);
    msg.id = sonar_front.message_id_distance;
    msg.data[0] = 0x02;
    msg.data[1] = 0x03;
    can_control.sendMessage(msg);

    pause();
}
