#include <iostream>
#include <iomanip>
#include "../lib/communication/canbus.h"
#include <libconfig.h++>
#include "../lib/configuration/Config.h"
#include "../lib/configuration/ConfigSection.h"
#include "../lib/devices/Sonar.h"

void register_new_frame(int message_id, const std::function<void(const can_msg *, std::error_code)> &handler_func,
                        CanBus &can_control, std::map<uint32_t, std::shared_ptr<io::CanRouterReadFrame>> &frames) {
    auto frame = std::make_shared<io::CanRouterReadFrame>(
        message_id,
        io::CanFlag::NONE,
        [handler_func, message_id, &can_control, &frames](
    const can_msg *msg, std::error_code ec) mutable {
            handler_func(msg, ec);
            can_control.registerReadFrame(*frames[message_id]);
        }
    );
    can_control.registerReadFrame(*frame);
    frames[message_id] = frame;
};


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

    std::map<uint32_t, std::shared_ptr<io::CanRouterReadFrame> > frames;

    std::vector<std::tuple<int, std::function<void(const can_msg *, std::error_code)> > > handlers = {
        {
            sonar_front.message_id_distance,
            [&](const can_msg *msg, std::error_code ec) { sonar_front.doStuffForSonar(msg, ec); }
        },
        {
            sonar_front.message_id_distance2,
            [&](const can_msg *msg, std::error_code ec) { sonar_front.doStuffForSonar_two(msg, ec); }
        },
    };

    for (auto &handler_entry: handlers) {
        int message_id = std::get<0>(handler_entry);
        auto &handler_func = std::get<1>(handler_entry);

        register_new_frame(message_id, handler_func, can_control, frames);
    }

    // set in can
    can_control.start();
    std::cout << "registered frame and started can" << std::endl;

    // sending message over the CAN bus
    can_msg msg{};
    msg.len = 8;
    msg.flags = static_cast<uint_least8_t>(io::CanFlag::NONE);
    msg.id = 212;
    msg.data[0] = 0x02;
    msg.data[1] = 0x03;
    can_control.sendMessage(msg);

    pause();
}
