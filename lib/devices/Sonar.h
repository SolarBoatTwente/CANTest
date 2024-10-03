#ifndef SONAR_H
#define SONAR_H
#include <cstdint>
#include <iostream>
#include <system_error>
#include <lely/can/msg.h>
#include <lely/io2/can_rt.hpp>
#include "../communication/canbus.h"
#include "../configuration/ConfigSection.h"
#include "../Device.h"


class Sonar : public Device {
public:
    Sonar(ConfigSection &config, CanBus &can) :
        Device(config, can, DeviceType::Sonar)
    {
        this->node_id = config.lookup("can.node_id");
        this->message_id_distance = static_cast<uint32_t>(config.lookup("can.message_id_distance")) | this->node_id;
        std::cout << "Sonar created" << std::endl ;
    }

    /**
     * Deconstructs a Sonar object
     */
    ~Sonar() = default;

    uint32_t message_id_distance;

    void doStuffForSonar(const can_msg *msg, std::error_code ec, lely::io::CanRouterReadFrame & frame) {
        std::cout << "Doing stuff here for the Sonar" << std::endl ;
        this->can_.registerReadFrame(frame);
    }

};



#endif //SONAR_H
