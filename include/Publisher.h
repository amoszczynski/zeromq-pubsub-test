#pragma once

#include <zmq.hpp>
#include <random>
#include "Data.h"

class Publisher {
public:
    Publisher(const std::string& address, int publisher_id);
    void run();
    void publish(const RawData& data);

private:
    zmq::context_t context;
    zmq::socket_t socket;
    int publisher_index;
    int m_seqnum;
    std::mt19937 gen;
    std::uniform_int_distribution<int> sleep_dist;
    std::uniform_int_distribution<int> increment_dist;
};
