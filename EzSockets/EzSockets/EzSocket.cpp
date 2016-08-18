//Author: Rémi Pelletier
//File:   EzSocket.cpp
//Desc.:  Definition of the EzSocket constructor as well as the
//        the "what" functions of the platform-independent exceptions.

#include "EzSocket.h"

namespace ez
{
    //Exceptions
    const char* SocketAcceptFailedException::what() const
    {
        return "An error occured when accepting a client connection.";
    }

    const char* SocketBindFailedException::what() const
    {
        return "An error occured when binding the socket.";
    }

    const char* SocketCloseFailedException::what() const
    {
        return "An error occured when closing the socket.";
    }

    const char* SocketCreationFailedException::what() const
    {
        return "An error occured when creating the socket.";
    }

    const char* SocketListenFailedException::what() const
    {
        return "An error occured when placing the socket in a listening state.";
    }

    const char* SocketReceiveFailedException::what() const
    {
        return "An error occured when receiving data with the socket.";
    }

    const char* SocketSendFailedException::what() const
    {
        return "An error occured when sending data with the socket.";
    }

    const char* SocketShutdownFailedException::what() const
    {
        return "An error occured when shutting down the socket.";
    }


    //EzSocket constructor
    EzSocket::EzSocket(SocketFamily family, SocketType type, SocketProtocol protocol)
        :m_family(family), m_type(type), m_protocol(protocol)
    {}

}