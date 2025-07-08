#include "Publisher.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <random>

Publisher::Publisher(const std::string& address, int publisher_id)
    : context(1),
      socket(context, ZMQ_PUB),
      publisher_index(publisher_id),
      m_seqnum(0),
      gen(std::random_device{}()),
      sleep_dist(100, 1000),
      increment_dist(1, 100)
{
    socket.connect(address);
}

void Publisher::run() {
    std::cout << "Publisher " << publisher_index << " started\n";
    
    while (true) {
        // Wait random amount of milliseconds
        int sleep_ms = sleep_dist(gen);
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
        
        // Create data with random increment
        RawData data;
        data.publisher_index = publisher_index;
        data.seq_num = m_seqnum++;
        data.increment = increment_dist(gen);
        
        // Publish the data
        std::string topic = "publisher_" + std::to_string(publisher_index);
        std::string message = topic + " " + data.serialize();
        
        zmq::message_t zmq_msg(message.size());
        memcpy(zmq_msg.data(), message.c_str(), message.size());
        socket.send(zmq_msg, zmq::send_flags::none);
        
        std::cout << "Publisher " << publisher_index << " sent: seq=" << data.seq_num 
                  << ", increment=" << data.increment << std::endl;
    }
}

void Publisher::publish(const RawData& data) {
    std::string topic = "publisher_" + std::to_string(data.publisher_index);
    std::string message = topic + " " + data.serialize();
    
    zmq::message_t zmq_msg(message.size());
    memcpy(zmq_msg.data(), message.c_str(), message.size());
    socket.send(zmq_msg, zmq::send_flags::none);
}
