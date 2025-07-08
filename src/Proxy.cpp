#include "Proxy.h"
#include <iostream>

Proxy::Proxy(const std::string& inAddress, const std::string& outAddress)
    : context(1),
      frontend(context, ZMQ_XSUB),
      backend(context, ZMQ_XPUB)
{
    frontend.bind(inAddress);
    backend.bind(outAddress);
}

void Proxy::start() {
    std::cout << "Starting proxy..." << std::endl;
    zmq::proxy(frontend, backend);
}