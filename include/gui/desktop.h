#ifndef __MYOS__GUI_DESKTOP_H
#define __MYOS__GUI_DESKTOP_H
/**********桌面**********/
#include "gui/widget.h"
#include "drivers/mouse.h"

namespace myos {
    namespace gui {
        //桌面类
        class Desktop : public CompositeWidget, public drivers::MouseEventHandler {
        public:
            Desktop(common::int32_t w, common::int32_t h, common::uint8_t r, common::uint8_t g, common::uint8_t b);
            ~Desktop();

            void Draw(common::GraphicsContext* gc) override;                //绘画
            void OnMouseDown(common::uint8_t button) override;              //鼠标按下事件
            void OnMouseUp(common::uint8_t button) override;                //鼠标抬起事件
            void OnMouseMove(common::int8_t x, common::int8_t y) override;  //鼠标移动事件
        private:
            common::uint32_t MouseX, MouseY;              //鼠标坐标
        };
    }
}

#endif 