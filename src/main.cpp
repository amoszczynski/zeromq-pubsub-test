#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <string>
#include <memory>

#include "Publisher.h"
#include "Subscriber.h"
#include "Proxy.h"

int main() {
    std::cout << "Starting ZeroMQ XPUB/XSUB Pattern Demo\n";
    std::cout << "=====================================\n";

    // Define addresses
    const std::string proxy_frontend = "tcp://*:5555";  // Publishers connect here
    const std::string proxy_backend = "tcp://*:5556";   // Subscribers connect here
    const std::string publisher_address = "tcp://localhost:5555";
    const std::string subscriber_address = "tcp://localhost:5556";

    try {
        // Start proxy in a separate thread
        std::cout << "Starting proxy...\n";
        Proxy proxy(proxy_frontend, proxy_backend);
        std::thread proxy_thread([&proxy]() {
            proxy.start();
        });

        // Give proxy time to start
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        // Create and start publishers in separate threads
        std::cout << "Starting publishers...\n";
        std::vector<std::thread> publisher_threads;
        std::vector<std::unique_ptr<Publisher>> publishers;

        for (int i = 0; i < 3; ++i) {
            publishers.push_back(std::make_unique<Publisher>(publisher_address, i));
            publisher_threads.emplace_back([&publishers, i]() {
                publishers[i]->run();
            });
        }

        // Give publishers time to start
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        // Create and start subscribers in separate threads
        std::cout << "Starting subscribers...\n";
        std::vector<std::thread> subscriber_threads;
        std::vector<std::unique_ptr<Subscriber>> subscribers;

        for (int i = 0; i < 2; ++i) {
            subscribers.push_back(std::make_unique<Subscriber>(subscriber_address));
            
            // Subscribe to specific publisher topics
            if (i == 0) {
                // First subscriber subscribes to publisher 0 and 1
                subscribers[i]->subscribe("publisher_0");
                subscribers[i]->subscribe("publisher_1");
                std::cout << "Subscriber 0 subscribed to publisher_0 and publisher_1\n";
            } else {
                // Second subscriber subscribes to publisher 1 and 2
                subscribers[i]->subscribe("publisher_1");
                subscribers[i]->subscribe("publisher_2");
                std::cout << "Subscriber 1 subscribed to publisher_1 and publisher_2\n";
            }

            subscriber_threads.emplace_back([&subscribers, i]() {
                std::cout << "Subscriber " << i << " started receiving...\n";
                subscribers[i]->receive();
            });
        }

        std::cout << "\nDemo is running. Press Enter to stop...\n";
        std::cin.get();

        std::cout << "Stopping demo...\n";
        
        // Note: In a real application, you would implement proper shutdown mechanisms
        // For this demo, we'll just exit (threads will be terminated)
        return 0;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
