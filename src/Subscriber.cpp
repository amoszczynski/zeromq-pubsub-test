#include "Subscriber.h"
#include <iostream>
#include <string>
#include <stdexcept>

Subscriber::Subscriber(const std::string& address)
    : context(1),
      socket(context, ZMQ_SUB)
{
    socket.connect(address);
}

void Subscriber::subscribe(const std::string& topic) {
    socket.set(zmq::sockopt::subscribe, topic);
}

void Subscriber::receive() {
    while (true) {
        zmq::message_t message;
        auto result = socket.recv(message, zmq::recv_flags::none);
        if (result) {
            std::string msg_str = message.to_string();
            RawData data = RawData::deserialize(msg_str.substr(msg_str.find(' ') + 1));
            std::cout << "Subscriber received increment: " << data.increment << std::endl;
        }
    }
}

RawData Subscriber::receiveData() {
    zmq::message_t message;
    auto result = socket.recv(message, zmq::recv_flags::none);
    if (result) {
        std::string msg_str = message.to_string();
        return RawData::deserialize(msg_str.substr(msg_str.find(' ') + 1));
    }
    throw std::runtime_error("Failed to receive message");
}
