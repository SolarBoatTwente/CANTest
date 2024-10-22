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


    std::vector<std::shared_ptr<io::CanRouterReadFrame>> frames;



    // Define the mapping of message IDs to handler functions
    std::vector<std::tuple<int, std::function<void(const can_msg*, std::error_code)>>> handlers = {
        {sonar_front.message_id_distance,       [&](const can_msg* msg, std::error_code ec) { sonar_front.doStuffForSonar(msg, ec); }},
        {sonar_front.message_id_distance2,      [&](const can_msg* msg, std::error_code ec) { sonar_front.doStuffForSonar_two(msg, ec); }},
        // Add other handler mappings here
    };

    // Function to create and register frames
    std::function<void(int, std::function<void(const can_msg*, std::error_code)>)> register_new_frame = [&](int message_id, std::function<void(const can_msg*, std::error_code)> handler_func) {
        // Create a shared pointer for each CanRouterReadFrame
        std::shared_ptr<io::CanRouterReadFrame> frame = std::make_shared<io::CanRouterReadFrame>(
            message_id,
            io::CanFlag::NONE,
            [handler_func, message_id, &can_control, register_new_frame, &frame](const can_msg* msg, std::error_code ec) mutable {
                // Call the corresponding handler function
                handler_func(msg, ec);

                // Re-register by creating and registering a new frame
                register_new_frame(message_id, handler_func);
            }
        );

        // Register the frame and store it
        can_control.registerReadFrame(*frame);
        frames.push_back(frame);  // Save frame to maintain ownership
    };

    // Loop through the handlers and create CanRouterReadFrame for each one
    for (auto& handler_entry : handlers) {
        int message_id = std::get<0>(handler_entry);
        auto& handler_func = std::get<1>(handler_entry);

        // Register the initial frame
        register_new_frame(message_id, handler_func);
    }



    //
    //
    // // Loop through the handlers and create CanRouterReadFrame for each one
    // for (auto& handler_entry : handlers) {
    //     int message_id = std::get<0>(handler_entry);
    //     auto& handler_func = std::get<1>(handler_entry);
    //
    //     // Create a shared pointer for each CanRouterReadFrame
    //     std::shared_ptr<io::CanRouterReadFrame> frame = std::make_shared<io::CanRouterReadFrame>(
    //         message_id,
    //         io::CanFlag::NONE,
    //         [frame, &handler_func, &can_control](const can_msg* msg, std::error_code ec) {
    //             // Call the corresponding handler function
    //             std::cout << "Test from inside the lambda" << std::endl;
    //             handler_func(msg, ec);
    //             can_control.registerReadFrame(*frame);
    //         }
    //     );
    //
    //     // Register the frame and store it
    //     can_control.registerReadFrame(*frame);
    //     frames.push_back(frame);
    // }
    //







    // std::list<struct io_can_rt_read_msg> frames;


    // {
    //     // Create a shared pointer for the CanRouterReadFrame
    //     shared_ptr<io::CanRouterReadFrame> read_frame = std::make_shared<io::CanRouterReadFrame>(
    //         sonar_front.message_id_distance,
    //         io::CanFlag::NONE,
    //         [&read_frame, &messageIds, &can_control](const can_msg* msg, std::error_code ec)
    //         {
    //             std::cout << "Test from inside the lambda" << std::endl;
    //             // auto it = messageIds.find(msg->id);
    //             // if (it != messageIds.end())
    //             // {
    //             //     it->second(msg, ec);
    //             // }
    //             can_control.registerReadFrame(*read_frame);
    //             // You don't need to re-register the frame inside the lambda
    //             return;
    //         }
    //     );
    //
    //     // Since it's now a shared_ptr, you can use the pointer safely
    //     frames.push_back(*read_frame); // Dereference to get the actual frame
    //     can_control.registerReadFrame(*read_frame);
    // }


    cout << frames.size() << std::endl;

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
