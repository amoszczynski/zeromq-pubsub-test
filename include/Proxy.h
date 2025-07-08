#pragma once

#include <zmq.hpp>
#include <string>

class Proxy {
public:
    Proxy(const std::string& inAddress, const std::string& outAddress);
    void start();

private:
    zmq::context_t context;
    zmq::socket_t frontend;
    zmq::socket_t backend;
};
