//
// Created by koen on 14-2-23.
//

#ifndef BOAT2023_CONFIGSECTION_H
#define BOAT2023_CONFIGSECTION_H

#include <libconfig.h++>
#include "Config.h"

class ConfigSection {
public:
    /**
     * Constructs a ConfigSection object
     * @param config Reference to the parent Config object
     * @param section the section in the config file
     */
    ConfigSection(Config& config, std::string section);

    /**
     * Deconstructs a ConfigSection object
     */
    ~ConfigSection() = default;

    /**
     * Looks up an entry in the section this object represents in the parent config.
     * @param entry the entry to look up
     * @return entry as a libconfig::Setting&
     */
    libconfig::Setting& lookup(const std::string& entry);
private:
    Config* config_;
    std::string section_;
};


#endif //BOAT2023_CONFIGSECTION_H
