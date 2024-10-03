//
// Created by koen on 5-12-22.
//

#ifndef BOAT2023_CANBUS_H
#define BOAT2023_CANBUS_H


#include <string>
#include <lely/io2/linux/can.hpp>
#include <lely/ev/loop.hpp>
#include <lely/io2/posix/poll.hpp>
#include <lely/io2/sys/io.hpp>
#include <lely/io2/can_rt.hpp>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <spdlog/spdlog.h>

using namespace lely;

class CanBus {
public:
    /**
     * Constructs a CanBus object
     * @param device_name the canbus interface name
     */
    explicit CanBus(const std::string &device_name);

    /**
     * Deconstructs a CanBus object
     */
    ~CanBus();

    /**
     * Sends a can message
     * @param can_message can message to send
     */
    void sendMessage(const can_msg& can_message);

    /**
     * Register a CanRouterReadFrame with the internal CanRouter
     * @param frame the CanRouterReadFrame to register
     */
    void registerReadFrame(io::CanRouterReadFrame& frame);

    /**
     * Formats a can message into a string
     * @param can_message can message
     * @return string representation of the can message
     */
    std::string formatMessage(const can_msg& can_message);

    /**
     * Starts the CanBus thread & event loop
     */
    void start();

    /**
     * Stops the CanBus thread & event loop
     */
    void stop();

    /**
     *
     * @return the internal io::Context
     */
    [[nodiscard]] const io::Context& getCtx() const;

    /**
     *
     * @return the internal io::CanController
     */
    [[nodiscard]] const io::CanController& getCanController() const;

protected:
    io::IoGuard io_guard_;
    io::Context ctx_;
    io::Poll poll_;
    ev::Loop loop_;
public:
    /**
     *
     * @return the internall io::Poll responsible for polling the event loop
     */
    const io::Poll &getPoll() const;

    /**
     *
     * @return the internal ev::Loop event loop
     */
    const ev::Loop &getLoop() const;

protected:
    io::CanController can_controller_;
    io::CanChannel can_channel_;
    io::CanRouter can_router_;
    std::thread event_loop_thread_;

    /**
     * Starts the thread responsible for reading & writing can messages
     */
    void startThread();
private:
    std::shared_ptr<spdlog::logger> logger_;
};


#endif //BOAT2023_CANBUS_H
