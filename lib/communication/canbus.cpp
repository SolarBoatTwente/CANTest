//
// Created by koen on 5-12-22.
//

#include "canbus.h"
#include <csignal>

CanBus::CanBus(const std::string &device_name) :
        poll_(ctx_, SIGEV_SIGNAL),
        loop_(poll_.get_poll()),
        can_controller_(device_name.c_str()),
        can_channel_(poll_, loop_.get_executor()),
        can_router_(can_channel_, loop_.get_executor()) {

    can_channel_.open(can_controller_);

    logger_ = spdlog::get("multi_sink");
}

CanBus::~CanBus() {
    loop_.stop();

    if (event_loop_thread_.joinable()) {
        event_loop_thread_.join();
    }
}

void CanBus::sendMessage(const can_msg& can_message) {
    try {
        can_channel_.write(can_message);
    } catch (const std::exception& e) {
        logger_->error(fmt::format("[{}] - {}"), this->can_controller_.get_name(), e.what());
    }
}

void CanBus::start() {
    event_loop_thread_ = std::thread(&CanBus::startThread, this);
}

void CanBus::startThread() {
    try {
        loop_.get_executor().on_task_init();
        loop_.run();
        loop_.get_executor().on_task_fini();
    } catch (const std::exception &e) {
        logger_->error(e.what());
    }
}

void CanBus::stop() {
    loop_.stop();
    event_loop_thread_.join();
    this->logger_->critical("aaah");
}

void CanBus::registerReadFrame(io::CanRouterReadFrame& frame) {
    can_router_.submit_read_frame(frame);
}

std::string CanBus::formatMessage(const can_msg &can_message) {
    return fmt::format("[{}] - {:x} [{}] {:x} {:x} {:x} {:x} {:x} {:x} {:x} {:x}", this->can_controller_.get_name(), can_message.id, can_message.len, can_message.data[0], can_message.data[1], can_message.data[2], can_message.data[3], can_message.data[4], can_message.data[5], can_message.data[6], can_message.data[7]);
}

const io::Context &CanBus::getCtx() const {
    return ctx_;
}

const io::CanController &CanBus::getCanController() const {
    return can_controller_;
}

const io::Poll &CanBus::getPoll() const {
    return poll_;
}

const ev::Loop &CanBus::getLoop() const {
    return loop_;
}
