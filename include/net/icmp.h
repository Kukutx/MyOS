#ifndef __MYOS__NET__ICMP_H
#define __MYOS__NET__ICMP_H
/**********ICMP协议**********/
#include "common/types.h"
#include "net/ipv4.h"

namespace myos {
    namespace net {
        struct InternetControlMessageProtocolMessage {
            common::uint8_t type;
            common::uint8_t code;
            common::uint16_t checkSum;

            common::uint32_t data;
        }__attribute__((packed));

        class InternetControlMessageProtocol : public InternetProtocolHandler {
        public:
            InternetControlMessageProtocol(InternetProtocolProvider* backend);
            ~InternetControlMessageProtocol();

            bool OnInternetProtocolReceived(common::uint32_t srcIP_BE, 
                common::uint32_t dstIP_BE,
                common::uint8_t* internetProtocolPayload, 
                common::uint32_t size);
            
            void RequestEchoReply(common::uint32_t ip_be);
        };
    }
}

#endif