#pragma once

#include <zmq.hpp>
#include "Data.h"

class Subscriber {
public:
    Subscriber(const std::string& address);
    void subscribe(const std::string& topic);
    void receive();
    RawData receiveData();

private:
    zmq::context_t context;
    zmq::socket_t socket;
};
