#ifndef __MYOS__NET__IPV4_H
#define __MYOS__NET__IPV4_H
/**********IPV4协议**********/
#include "common/types.h"
#include "net/etherframe.h"
#include "net/arp.h"

namespace myos {
    namespace net {
        struct InternetProtocolV4Message {
            common::uint8_t headerLength : 4;
            common::uint8_t version : 4;
            common::uint8_t tos;
            common::uint16_t totalLength;

            common::uint16_t ident;
            common::uint16_t flagsAndOffset;

            common::uint8_t timeToLive;
            common::uint8_t protocol;
            common::uint16_t checkSum;
            
            common::uint32_t srcIP;

            common::uint32_t dstIP;
        }__attribute__((packed));

        class InternetProtocolProvider;

        class InternetProtocolHandler {
        public:
            InternetProtocolHandler(InternetProtocolProvider* backend, common::uint8_t protocol);
            ~InternetProtocolHandler();

            virtual bool OnInternetProtocolReceived(common::uint32_t srcIP_BE, 
                common::uint32_t dstIP_BE,
                common::uint8_t* internetProtocolPayload, 
                common::uint32_t size);

            void Send(common::uint32_t dstIP_BE, common::uint8_t* internetProtocolPayload, common::uint32_t size);
        protected:
            InternetProtocolProvider* backend;
            common::uint8_t protocol;
        };

        class InternetProtocolProvider : public EtherFrameHandler {
            friend class InternetProtocolHandler;
        public:
            InternetProtocolProvider(EtherFrameProvider* backend, 
                AddressResolutionProtocol* arp,
                common::uint32_t gatewayIP,
                common::uint32_t subnetMask);

            ~InternetProtocolProvider();

            bool OnEtherFrameReceived(common::uint8_t* buffer, common::uint32_t size);
            void Send(common::uint32_t dstIP_BE, common::uint8_t protocol, common::uint8_t* buffer, common::uint32_t size);
            static common::uint16_t CheckSum(common::uint16_t* data, common::uint32_t size);

        protected:
            InternetProtocolHandler* handlers[255];
            AddressResolutionProtocol* arp;
            common::uint32_t gatewayIP;
            common::uint32_t subnetMask;
        };
    }
}

#endif