#pragma once

#include <string>
#include <sstream>

struct RawData {
    int publisher_index;
    int seq_num;
    int increment;

    std::string serialize() const {
        std::ostringstream oss;
        oss << publisher_index << "," << seq_num << "," << increment;
        return oss.str();
    }

    static RawData deserialize(const std::string& data) {
        std::istringstream iss(data);
        RawData rawData;
        char delimiter;
        iss >> rawData.publisher_index >> delimiter >> rawData.seq_num >> delimiter >> rawData.increment;
        return rawData;
    }
};
