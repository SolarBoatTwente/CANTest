//
// Created by koen on 13-2-23.
//

#include <libconfig.h++>
#include <shared_mutex>
#include <spdlog/spdlog.h>

#ifndef BOAT2023_CONFIG_H
#define BOAT2023_CONFIG_H

using namespace std;
using namespace chrono;

class Config {
public:
    using ConfigMutex = std::shared_mutex;

private:
    mutable ConfigMutex configMutex;
    libconfig::Config config_;
    std::shared_ptr<spdlog::logger> logger_;

public:
    /**
     * Constructs a Config object by reading a libconfig config file
     * @param filename filename of the config file, relative to the build directory
     */
    Config(const std::string& filename);

    /**
     * Deconstructs a Config object
     */
    ~Config() = default;

    /**
     * Read a libconfig config file, replacing any previous configuration
     * @param filename filename of the config file, relative to the build directory
     */
    void readFile(const std::string& filename);


    /**
     * Reads an entry from the root in the config file
     * @param entry entry to read
     * @return entry as a libconfig::Setting&
     */
    libconfig::Setting& read(const std::string& entry);

    /**
     * Reads an entry from the root in the config file
     * @param entry entry to read
     * @return entry as a libconfig::Setting&
     */
    libconfig::Setting& lookup(const std::string& section, const std::string& entry);
};


#endif //BOAT2023_CONFIG_H
