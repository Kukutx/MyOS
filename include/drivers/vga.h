#ifndef __MYOS__HARDWARECOMMUNICATION__VGA_H
#define __MYOS__HARDWARECOMMUNICATION__VGA_H
/**********VGA接口(视频图形阵列)**********/
/*可查看 VGA 寄存器一览表：https://blog.csdn.net/cosmoslife/article/details/9179233*/
#include "common/types.h"
#include "hardwarecommunication/port.h"
#include "drivers/driver.h"
/**/
namespace myos {
    namespace drivers {
        //视频图形阵列类
        class VideoGraphicsArray {
        public:
            VideoGraphicsArray();
            ~VideoGraphicsArray();

            bool SupportsMode(common::uint32_t width, common::uint32_t height, common::uint32_t colordepth);              //支持模式
            bool SetMode(common::uint32_t width, common::uint32_t height, common::uint32_t colordepth);                   //设置模式
            void PutPixel(common::int32_t x, common::int32_t y, common::uint8_t r, common::uint8_t g, common::uint8_t b); //放置像素
            void PutPixel(common::int32_t x, common::int32_t y, common::uint8_t colorindex);                              //放置像素
            void FillRectangle(common::uint32_t x, common::uint32_t y, common::uint32_t w, common::uint32_t h,            //填充矩形
            common::uint8_t r, common::uint8_t g, common::uint8_t b);
        private:
            hardwarecommunication::Port8Bit miscPort;                                //混合输出端口
            hardwarecommunication::Port8Bit crtcIndexPort;                           //CRT控制索引端口
            hardwarecommunication::Port8Bit crtcDataPort;                            //CRT控制显示数据端口
            hardwarecommunication::Port8Bit sequencerIndexPort;                      //定序器索引端口       
            hardwarecommunication::Port8Bit sequencerDataPort;                       //定序器数据端口
            hardwarecommunication::Port8Bit graphicsControllerIndexPort;             //图形控制索引端口
            hardwarecommunication::Port8Bit graphicsControllerDataPort;              //图形控制数据端口
            hardwarecommunication::Port8Bit attributeControllerIndexPort;            //属性控制索引端口
            hardwarecommunication::Port8Bit attributeControllerReadPort;             //属性控制读取端口
            hardwarecommunication::Port8Bit attributeControllerWritePort;            //属性控制写入端口
            hardwarecommunication::Port8Bit attributeControllerResetPort;            //属性控制重置端口


            void WriteRegisters(common::uint8_t* registers);                         //写寄存器
            common::uint8_t GetColorIndex(common::uint8_t r, common::uint8_t g, common::uint8_t b);   //获取颜色索引
            common::uint8_t* GetFrameBufferSegment();                                //获取帧缓冲区段
        };
    }
}


#endif 