#ifndef __MYOS__NET__ARP_H
#define __MYOS__NET__ARP_H
/**********arp协议**********/
#include "common/types.h"
#include "net/etherframe.h"

namespace myos {
    namespace net {
        // 地址解析协议头
        struct AddressResolutionProtocolMessage {
            common::uint16_t hardwareType;
            common::uint16_t protocol;
            common::uint8_t hardwareAddressSize;
            common::uint8_t protocolAddressSize;
            common::uint16_t command;

            common::uint64_t srcMAC : 48;
            common::uint32_t srcIP;
            common::uint64_t dstMAC : 48;
            common::uint32_t dstIP;
        }__attribute__((packed));
        // 地址解析协议
        class AddressResolutionProtocol : public EtherFrameHandler {
        public:
            AddressResolutionProtocol(EtherFrameProvider* backend);
            ~AddressResolutionProtocol();
            //接收到的以太帧
            bool OnEtherFrameReceived(common::uint8_t* etherframePayload, common::uint32_t size);
            //请求MAC地址
            void RequestMACAddress(common::uint32_t IP_BE);
            common::uint64_t Resolve(common::uint32_t IP_BE);         //解析
            common::uint64_t GetMACFromCache(common::uint32_t IP_BE); //从缓存中获取MAC
            void BroadcastMACAddress(common::uint32_t IP_BE);         //广播MAC地址
        private:
            common::uint32_t IPcache[128];
            common::uint64_t MACcache[128];
            int numCacheEntries;
        };
    }
}

#endif