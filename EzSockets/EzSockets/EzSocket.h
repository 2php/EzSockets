//Author: Rémi Pelletier
//File:   EzSocket.h
//Desc.:  Declaration of the abstract base class EzSocket along
//        with platform-independent constants and exceptions.  

#pragma once
#include <cstdint>
#include <exception>
#include <iostream>
#include <string>

//--Project tasks--
//TODO: Add POSIX sockets support.
//TODO: Add functionalities.

//--EzSocket.h tasks--
//TODO: Review the name of the constants (in the enums).


namespace ez 
{
    class SocketAcceptFailedException : public std::exception
    {
    public:
        virtual const char* what() const;
    };

    class SocketBindFailedException : public std::exception
    {
    public:
        virtual const char* what() const;
    };

    class SocketCloseFailedException : public std::exception
    {
    public:
        virtual const char* what() const;
    };

    class SocketCreationFailedException : public std::exception
    {
    public:
        virtual const char* what() const;
    };

    class SocketListenFailedException : public std::exception
    {
    public:
        virtual const char* what() const;
    };

    class SocketReceiveFailedException : public std::exception
    {
    public:
        virtual const char* what() const;
    };

    class SocketSendFailedException : public std::exception
    {
    public:
        virtual const char* what() const;
    };

    class SocketShutdownFailedException : public std::exception
    {
    public:
        virtual const char* what() const;
    };


    enum SocketFamily
    {
        FAMILY_AF_INET,
        FAMILY_AF_INET6,
        FAMILY_AF_UNIX,
        FAMILY_AF_UNSPEC,
        FAMILY_AF_NETBIOS
    };

    enum SocketType
    {
        TYPE_SOCK_STREAM,
        TYPE_SOCK_DGRAM,
        TYPE_SOCK_RAW,
        TYPE_SOCK_RDM,
        TYPE_SOCK_SEQPACKET
    };

    enum SocketProtocol
    {
        PROTO_IPPROTO_TCP,
        PROTO_IPPROTO_UDP
    };

    enum SocketShutdownFlag
    {
        SHUT_RECV,
        SHUT_SEND,
        SHUT_BOTH
    };


    class EzSocket
    {
    protected:
        std::string m_address = "";
        uint32_t    m_port = EzSocket::DEFAULT_PORT;

        SocketFamily   m_family;
        SocketType     m_type;
        SocketProtocol m_protocol;


    public:
        static const uint32_t DEFAULT_PORT = 8080;

        EzSocket(SocketFamily family, SocketType type, SocketProtocol protocol);

        virtual void init() = 0;
        virtual void cleanup() = 0;

        virtual void accept() = 0;
        virtual void bind(const std::string& addr, const std::string& port) = 0;
        virtual void bind(const std::string& addr, uint32_t port) = 0;
        virtual void close() = 0;
        virtual void connect(const std::string& addr, uint32_t port) = 0;
        //virtual void getsockname() const = 0;
        //virtual void getsockopt() const = 0;
        virtual void listen() = 0;
        virtual int recv(char* buffer, int bufferLength, bool& connectionClosed) = 0;
        //virtual int recvfrom(char* buffer, int bufferLength, bool& connectionClosed) = 0;
        //virtual void select() = 0;
        virtual void send(const char* buffer, int bufferLength) = 0;
        virtual void send(std::istream& input) = 0;
        //virtual void sendto(char* buffer, int bufferLength) = 0;
        virtual void shutdown(SocketShutdownFlag how = SocketShutdownFlag::SHUT_BOTH) = 0;
    };

    std::string gethostname();
    //void gethostbyaddr(const std::string& addr);
    //std::string gethostbyname(const std::string& name);

    EzSocket* createSocket(SocketFamily family, SocketType type, SocketProtocol protocol);
    void destroySocket(EzSocket* socket);
}