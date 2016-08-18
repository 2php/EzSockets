//Author: Rémi Pelletier
//File:   main.cpp
//Desc.:  Program entry point.

#include <iostream>
#include <string>
#include "EzSockets.h"


int main()
{
    //Simple server test.
    ez::EzSocket* socket = ez::createSocket(ez::FAMILY_AF_INET,
                                            ez::TYPE_SOCK_STREAM,
                                            ez::PROTO_IPPROTO_TCP);

    std::cout << "Init..." << std::endl;
    socket->init();

    std::cout << "Bind..." << std::endl;
    socket->bind(ez::gethostname(), 8080);

    std::cout << "Listen..." << std::endl;
    socket->listen();

    std::cout << "Accept..." << std::endl;
    socket->accept();

    std::string message("Hello!");
    std::cout << "Send..." << std::endl;
    socket->send(message.c_str(), message.length());

    std::cout << "Shutdown..." << std::endl;
    socket->shutdown();

    std::cout << "Close..." << std::endl;
    socket->close();

    std::cout << "Cleanup..." << std::endl;
    socket->cleanup();

    ez::destroySocket(socket);

    return 0;
}