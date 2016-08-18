//Author: Rémi Pelletier
//File:   EzSocket_Win.cpp
//Desc.:  Definition of the EzSocket_Win class as well Winsock
//        related functions and exceptions.

#ifdef _WIN32

#include <sstream>
#include "EzSocket_Win.h"

//TODO: Review error handling.
//TODO: Review socket initialization and destruction.
//TODO: Review Winsock initialization and cleanup.

namespace ez
{
    //Winsock related exceptions
    const char* WinsockInitFailedException::what() const
    {
        return "An error occured when initializing Winsock.";
    }

    const char* AddressResolveFailedException::what() const
    {
        return "An error occured when resolving the address info.";
    }


    //Winsock functions
    void initWinsock()
    {
        WSADATA wsaData;
        int iResult = ::WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iResult != NO_ERROR) {
            throw WinsockInitFailedException();
        }
    }

    void cleanupWinsock()
    {
        WSACleanup();
    }

    int getWinsockFamily(SocketFamily family)
    {
        int val;
        switch (family) {
        case SocketFamily::FAMILY_AF_INET:
            val = AF_INET;
            break;
        case SocketFamily::FAMILY_AF_INET6:
            val = AF_INET6;
            break;
        case SocketFamily::FAMILY_AF_UNIX:
            val = AF_UNIX;
            break;
        case SocketFamily::FAMILY_AF_UNSPEC:
            val = AF_UNSPEC;
            break;
        case SocketFamily::FAMILY_AF_NETBIOS:
            val = AF_NETBIOS;
            break;
        default:
            val = AF_INET;
            break;
        }
        return val;
    }

    int getWinsockType(SocketType type)
    {
        int val;
        switch (type) {
        case SocketType::TYPE_SOCK_STREAM:
            val = SOCK_STREAM;
            break;
        case SocketType::TYPE_SOCK_DGRAM:
            val = SOCK_DGRAM;
            break;
        case SocketType::TYPE_SOCK_RAW:
            val = SOCK_RAW;
            break;
        case SocketType::TYPE_SOCK_RDM:
            val = SOCK_RDM;
            break;
        case SocketType::TYPE_SOCK_SEQPACKET:
            val = SOCK_SEQPACKET;
            break;
        default:
            val = SOCK_STREAM;
            break;
        }
        return val;
    }

    int getWinsockProtocol(SocketProtocol protocol)
    {
        int val;
        switch (protocol) {
        case SocketProtocol::PROTO_IPPROTO_TCP:
            val = IPPROTO_TCP;
            break;
        case SocketProtocol::PROTO_IPPROTO_UDP:
            val = IPPROTO_UDP;
            break;
        default:
            val = IPPROTO_TCP;
            break;
        }
        return val;
    }

    int getWinsockShutdownFlag(SocketShutdownFlag flag)
    {
        int how;
        switch (flag) {
        case SocketShutdownFlag::SHUT_RECV:
            how = SD_RECEIVE;
            break;
        case SocketShutdownFlag::SHUT_SEND:
            how = SD_SEND;
            break;
        case SocketShutdownFlag::SHUT_BOTH:
            how = SD_BOTH;
            break;
        }
        return how;
    }


    //Private methods
    void EzSocket_Win::setupHints()
    {
        ZeroMemory(&m_hints, sizeof(m_hints));
        m_hints.ai_family   = getWinsockFamily(m_family);
        m_hints.ai_socktype = getWinsockType(m_type);
        m_hints.ai_protocol = getWinsockProtocol(m_protocol);
    }

    void EzSocket_Win::resolveAddress(PCSTR addr, PCSTR port)
    {
        int iResult = ::getaddrinfo(addr, port, &m_hints, &m_resultAddr);
        if (iResult != 0) {
            throw AddressResolveFailedException();
        }
    }

    void EzSocket_Win::createSocket(struct addrinfo* addrPtr)
    {
        m_winsocket = ::socket(addrPtr->ai_family, 
                               addrPtr->ai_socktype,
                               addrPtr->ai_protocol);
        if (m_winsocket == INVALID_SOCKET) {
            ::freeaddrinfo(m_resultAddr);
            throw SocketCreationFailedException();
        }
    }


    //Constructor and destructor
    EzSocket_Win::EzSocket_Win(SocketFamily   family, 
                               SocketType     type,
                               SocketProtocol protocol)
        :EzSocket(family, type, protocol)
    {}

    EzSocket_Win::~EzSocket_Win() {}


    //Public methods
    void EzSocket_Win::init()
    {
        initWinsock(); //TODO: Check if already initialized.
        setupHints();
    }

    void EzSocket_Win::cleanup()
    {
        cleanupWinsock(); //TODO: Check if already cleaned up.
    }

    void EzSocket_Win::accept()
    {
        SOCKET tmpSocket = ::accept(m_winsocket, NULL, NULL);
        if (tmpSocket == INVALID_SOCKET) {
            ::closesocket(m_winsocket);
            throw SocketAcceptFailedException();
        }
        ::closesocket(m_winsocket);
        m_winsocket = tmpSocket;
    }

    void EzSocket_Win::bind(const std::string& addr, const std::string& port)
    {
        resolveAddress(addr.c_str(), port.c_str());
        createSocket(m_resultAddr);

        int iResult = ::bind(m_winsocket, m_resultAddr->ai_addr,
                             m_resultAddr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            ::freeaddrinfo(m_resultAddr);
            ::closesocket(m_winsocket);
            throw SocketBindFailedException();
        }
        ::freeaddrinfo(m_resultAddr);
    }

    void EzSocket_Win::bind(const std::string& addr, uint32_t port)
    {
        EzSocket_Win::bind(addr, std::to_string(port));
    }

    void EzSocket_Win::close()
    {
        int iResult = ::closesocket(m_winsocket);
        if (iResult == SOCKET_ERROR) {
            throw SocketCloseFailedException();
        }
    }

    void EzSocket_Win::connect(const std::string& addr, const std::string& port)
    {
        resolveAddress(addr.c_str(), port.c_str());
        struct addrinfo* addrPtr = NULL;

        int iResult;
        for (addrPtr = m_resultAddr; addrPtr != NULL; addrPtr = addrPtr->ai_next) {
            createSocket(addrPtr);
            iResult = ::connect(m_winsocket, addrPtr->ai_addr, 
                                (int)addrPtr->ai_addrlen);
            if (iResult == SOCKET_ERROR) {
                ::closesocket(m_winsocket);
                m_winsocket = INVALID_SOCKET;
                continue;
            }
            break;
        }

        ::freeaddrinfo(m_resultAddr);

        if (m_winsocket == INVALID_SOCKET) {
            throw SocketCreationFailedException();
        }
    }

    void EzSocket_Win::connect(const std::string& addr, uint32_t port)
    {
        EzSocket_Win::connect(addr, std::to_string(port));
    }

    //void EzSocket_Win::getsockname() const
    //{
    //
    //}
    //
    //void EzSocket_Win::getsockopt() const
    //{
    //
    //}

    void EzSocket_Win::listen()
    {
        int iResult = ::listen(m_winsocket, SOMAXCONN);
        if (iResult == SOCKET_ERROR) {
            ::closesocket(m_winsocket);
            throw SocketListenFailedException();
        }
    }

    int EzSocket_Win::recv(char* buffer, int bufferLength, bool& connectionClosed)
    {
        if (buffer == nullptr || bufferLength == 0) {
            return 0;
        }

        connectionClosed = false;

        int bytesReceived = 0;
        int iResult;
        do {
            iResult = ::recv(m_winsocket, buffer + bytesReceived,
                             bufferLength - bytesReceived, 0);
            if (iResult == SOCKET_ERROR) {
                ::closesocket(m_winsocket);
                throw SocketReceiveFailedException();
            }
            else if (iResult == 0) {
                connectionClosed = true;
                break;
            }
            bytesReceived += iResult;
        } while (bytesReceived < bufferLength);

        return bytesReceived;
    }

    //int EzSocket_Win::recvfrom()
    //{
    //
    //}
    //
    //void EzSocket_Win::select()
    //{
    //
    //}

    void EzSocket_Win::send(const char* buffer, int bufferLength)
    {
        if (buffer == nullptr || bufferLength == 0) {
            return;
        }

        int bytesSent = 0;
        int iResult;
        do {
            iResult = ::send(m_winsocket, buffer + bytesSent,
                             bufferLength - bytesSent, 0);
            if (iResult == SOCKET_ERROR) {
                ::closesocket(m_winsocket);
                throw SocketSendFailedException();
            }
            bytesSent += iResult;
        } while (bytesSent < bufferLength);
    }

    void EzSocket_Win::send(std::istream& input)
    {
        std::stringstream buff;
        buff << input.rdbuf();
        buff.seekg(0, std::ios::end);
        int inputSize = buff.tellg();
        EzSocket_Win::send(buff.str().c_str(), inputSize);
    }

    //void EzSocket_Win::sendto()
    //{
    //
    //}

    void EzSocket_Win::shutdown(SocketShutdownFlag how)
    {
        int iResult = ::shutdown(m_winsocket, getWinsockShutdownFlag(how));
        if (iResult == SOCKET_ERROR) {
            ::closesocket(m_winsocket);
            throw SocketShutdownFailedException();
        }
    }


    //External socket functions
    std::string gethostname()
    {
        //TODO: Check if Winsock is initialized.
        static const int HOST_NAME_BUFFER_LEN = 256; //Always adequate.
        char hostNameBuffer[HOST_NAME_BUFFER_LEN];
        ::gethostname(hostNameBuffer, HOST_NAME_BUFFER_LEN);
        return std::string(hostNameBuffer);
    }

    //void gethostbyaddr(const std::string& addr)
    //{
    //
    //}
    //
    //std::string gethostbyname(const std::string& name)
    //{
    //    struct hostent* host = ::gethostbyname(name.c_str());
    //    return std::string(host->h_addr_list[0]);
    //}


    //EzSocket factory functions
    EzSocket* createSocket(SocketFamily   family, 
                           SocketType     type,
                           SocketProtocol protocol)
    {
        return new EzSocket_Win(family, type, protocol);
    }

    void destroySocket(EzSocket* socket)
    {
        delete socket;
    }

}

#endif