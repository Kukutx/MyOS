#ifndef __MYOS__GUI_WINDOW_H
#define __MYOS__GUI_WINDOW_H
/**********窗口**********/
#include "gui/widget.h"
#include "drivers/mouse.h"

namespace myos {
    namespace gui { 
        //窗口类
        class Window : public CompositeWidget {
        public:
            Window(Widget* parent, common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h,
                common::uint8_t r, common::uint8_t g, common::uint8_t b);
            ~Window();

            void OnMouseDown(common::int32_t x, common::int32_t y, common::uint8_t button) override;                       //鼠标按下事件接口
            void OnMouseUp(common::int32_t x, common::int32_t y, common::uint8_t button) override;                         //鼠标抬起事件接口
            void OnMouseMove(common::int32_t x, common::int32_t y, common::int32_t nx, common::int32_t ny) override;       //鼠标移动事件接口
        private:
            bool Dragging;                                                                                                 //拖动
        };
    }
}

#endif 