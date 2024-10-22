#include <iostream>
#include <iomanip>
#include "../lib/communication/canbus.h"
#include <libconfig.h++>
#include "../lib/configuration/Config.h"
#include "../lib/configuration/ConfigSection.h"
#include "../lib/devices/Sonar.h"

int main()
{
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

    //create map with endpoints
    map<int, std::function<void(const can_msg* msg, std::error_code ec)>> messageIds = {
        {
            sonar_front.message_id_distance,
            [&](const can_msg* msg, std::error_code ec) { sonar_front.doStuffForSonar(msg, ec); }
        }
    };

    std::list<struct io_can_rt_read_msg> frames;


    io::CanRouterReadFrame read_frame(sonar_front.message_id_distance, io::CanFlag::NONE,
                                      [&](const can_msg* msg, std::error_code ec)
                                      {
                                          messageIds.find(msg->id)->second(msg, ec);
                                          can_control.registerReadFrame(read_frame);
                                          return;
                                      });
    frames.push_back(read_frame);
    can_control.registerReadFrame(read_frame);


    cout << frames.size() << std::endl;


    // can_control.registerReadFrame(frames.back());


    // create function
    // io::CanRouterReadFrame read_frame(sonar_front.message_id_distance, io::CanFlag::NONE,
    //                                              [&](const can_msg* msg, std::error_code ec)
    //                                              {
    //                                                  messageIds.find(msg->id)->second(msg, ec);
    //                                                  // sonar_front.doStuffForSonar(msg, ec);
    //                                                  can_control.registerReadFrame(read_frame);
    //                                                  return;
    //                                              });

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
