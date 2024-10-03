//
// Created by koen on 9-12-22.
//

#ifndef BOAT2023_DEVICE_H
#define BOAT2023_DEVICE_H

#include "communication/canbus.h"

#include "configuration/ConfigSection.h"
#include <spdlog/spdlog.h>
#include <mqtt/async_client.h>

enum DeviceType {
    Sonar
};

class Device {
public:
    /**
     * Constructs a Device object
     * @param config the ConfigSection with the settings for this Device
     * @param can the CanBus this Device is operating on
     * @param client the shared mqtt::async_client
     * @param type type of Device as a DeviceType
     */
    Device(ConfigSection &config, CanBus &can, DeviceType type);

    /**
     * Deconstructs a Device object
     */
    ~Device();

    ConfigSection& config_;
    CanBus& can_;
    DeviceType type_;
    uint32_t node_id;
};


#endif //BOAT2023_DEVICE_H
