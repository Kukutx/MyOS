#ifndef __MYOS__NET__UDP_H
#define __MYOS__NET__UDP_H
/**********UDP协议**********/
#include "common/types.h"
#include "net/ipv4.h"
#include "memorymanagement.h"

namespace myos {
    namespace net {

        struct UserDatagramProtocolHeader {
            common::uint16_t srcPort;
            common::uint16_t dstPort;
            common::uint16_t length;
            common::uint16_t checkSum;
        }__attribute__((packed));

        class UserDatagramProtocolProvider;
        class UserDatagramProtocolSocket;

        class UserDatagramProtocolHandler {
        public:
            UserDatagramProtocolHandler();
            ~UserDatagramProtocolHandler();

            virtual void HandleUserDatagramProtocolMessage(UserDatagramProtocolSocket* socket, common::uint8_t* data, common::uint16_t size);
        };

        class UserDatagramProtocolSocket {
            friend class UserDatagramProtocolProvider;
        public:
            UserDatagramProtocolSocket(UserDatagramProtocolProvider* backend);
            ~UserDatagramProtocolSocket();

            virtual void HandleUserDatagramProtocolMessage(common::uint8_t* data, common::uint16_t size);
            virtual void Send(common::uint8_t* data, common::uint16_t size);
            virtual void Disconnect();

        protected:
            common::uint16_t remotePort;
            common::uint16_t remoteIP;
            common::uint16_t localPort;
            common::uint16_t localIP;

            bool listening;

            UserDatagramProtocolHandler* handler;
            UserDatagramProtocolProvider* backend;
        };

        class UserDatagramProtocolProvider : public InternetProtocolHandler {
        public:
            UserDatagramProtocolProvider(InternetProtocolProvider* backend);
            ~UserDatagramProtocolProvider();

            virtual bool OnInternetProtocolReceived(common::uint32_t srcIP_BE, 
                common::uint32_t dstIP_BE,
                common::uint8_t* internetProtocolPayload, 
                common::uint32_t size);
            virtual UserDatagramProtocolSocket* Connect(common::uint32_t ip, common::uint16_t port);
            virtual UserDatagramProtocolSocket* Listen(common::uint16_t port);
            virtual void Disconnect(UserDatagramProtocolSocket* socket);
            virtual void Send(UserDatagramProtocolSocket* socket, common::uint8_t* data, common::uint16_t size);
            virtual void Bind(UserDatagramProtocolSocket* socket, UserDatagramProtocolHandler* handler);

        protected:
            UserDatagramProtocolSocket* sockets[65535];
            common::uint16_t numSockets;
            common::uint16_t freePort;
        };

    }
}

#endif