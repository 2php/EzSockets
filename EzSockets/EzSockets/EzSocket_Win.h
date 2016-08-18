//Author: Rémi Pelletier
//File:   EzSocket_Win.h
//Desc.:  Declaration of the EzSocket_Win class used for the Winsock
//        EzSocket implementation along with various Winsock related
//        functions and exceptions.

#pragma once
#ifdef _WIN32

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include "EzSocket.h"

#pragma comment (lib, "Ws2_32.lib")

namespace ez
{
    class WinsockInitFailedException
    {
    public:
        virtual const char* what() const;
    };

    class AddressResolveFailedException
    {
    public:
        virtual const char* what() const;
    };


    void initWinsock();
    void cleanupWinsock();

    int getWinsockFamily(SocketFamily family);
    int getWinsockType(SocketType type);
    int getWinsockProtocol(SocketProtocol protocol);
    int getWinsockShutdownFlag(SocketShutdownFlag flag);


    class EzSocket_Win : public EzSocket
    {
    private:
        SOCKET m_winsocket = INVALID_SOCKET;
        struct addrinfo* m_resultAddr = NULL;
        struct addrinfo  m_hints;

        void setupHints();
        void resolveAddress(PCSTR addr, PCSTR port);
        void createSocket(struct addrinfo* addrPtr);

    public:
        EzSocket_Win(SocketFamily   family, 
                     SocketType     type, 
                     SocketProtocol protocol);
        ~EzSocket_Win();

        virtual void init();
        virtual void cleanup();

        virtual void accept();
        virtual void bind(const std::string& addr, const std::string& port);
        virtual void bind(const std::string& addr, uint32_t port);
        virtual void close();
        virtual void connect(const std::string& addr, const std::string& port);
        virtual void connect(const std::string& addr, uint32_t port);
        //virtual void getsockname() const;
        //virtual void getsockopt() const;
        virtual void listen();
        virtual int recv(char* buffer, int bufferLength, bool& connectionClosed);
        //virtual int recvfrom(char* buffer, int bufferLength, bool& connectionClosed);
        //virtual void select();
        virtual void send(const char* buffer, int bufferLength);
        virtual void send(std::istream& input);
        //virtual void sendto(char* buffer, int bufferLength);
        virtual void shutdown(SocketShutdownFlag how);
    };
}

#endif