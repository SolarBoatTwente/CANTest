//
// Created by koen on 9-12-22.
//

#include "Device.h"

Device::Device(ConfigSection &config, CanBus &can, DeviceType type) : config_(config), can_(can), type_(type)
{
    std::cout << "Device created" << std::endl ;
}



Device::~Device() = default;

