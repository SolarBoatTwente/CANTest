//
// Created by koen on 14-2-23.
//

#include "ConfigSection.h"

#include <utility>

ConfigSection::ConfigSection(Config &config, std::string section) {
  this->config_ = &config;
  this->section_ = std::move(section);
}

libconfig::Setting &ConfigSection::lookup(const std::string& entry) {
    auto logger_ = spdlog::get("brainpi");
    try {
        return this->config_->lookup(section_, entry);;
    } catch (libconfig::SettingNotFoundException &e) {
        logger_->critical(e.getPath());
    }
}
