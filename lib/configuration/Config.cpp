//
// Created by koen on 13-2-23.
//

#include "Config.h"
#include <mutex>


libconfig::Setting &Config::read(const std::string& entry) {
    std::shared_lock _(configMutex); //Scoped lock, will unlock when it runs out of scope.
    try {
        libconfig::Setting& value = this->config_.getRoot().lookup(entry);
        return value;
    } catch (libconfig::SettingNotFoundException &e) {
        this->logger_->critical(e.getPath());
    }
}

Config::Config(const std::string& filename) {
    logger_ = spdlog::get("brainpi");
    this->readFile(filename);
}

void Config::readFile(const std::string &filename) {
    std::unique_lock _(configMutex); //Scoped lock, will unlock when it runs out of scope.
    try {
        this->config_.readFile(filename);
        return;
    } catch (const libconfig::FileIOException &fioex) {
        logger_->error("I/O error while reading config file.");
    } catch (const libconfig::ParseException &pex) {
        logger_->error(fmt::format("Parse error at {} : {} - {}", pex.getFile(), pex.getLine(), pex.getError()));
    }
}

libconfig::Setting &Config::lookup(const string &section, const string &entry) {
    std::shared_lock _(configMutex); //Scoped lock, will unlock when it runs out of scope.
    libconfig::Setting& value =  this->read(section).lookup(entry);
    return value;
}
