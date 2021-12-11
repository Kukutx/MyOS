#ifndef __MYOS__GUI_WIDGET_H
#define __MYOS__GUI_WIDGET_H
/**********部件**********/
#include "common/types.h"
#include "common/graphicscontext.h"
#include "drivers/keyboard.h"

namespace myos {
    namespace gui {
        //部件类
        class Widget : public drivers::KeyboardEventHandler {
        public:
            Widget(Widget* parent, common::int32_t x, common::int32_t y,
                common::int32_t w, common::int32_t h, common::uint8_t r, 
                common::uint8_t g, common::uint8_t b);
            ~Widget();

            virtual void GetFocus(Widget* widget);                                                             //获取焦点接口
            virtual void ModelToScreen(common::int32_t &x, common::int32_t &y);                                //箭头到屏幕接口
            virtual bool ContainsCoordinate(common::int32_t x, common::int32_t y);                             //包含坐标接口

            virtual void Draw(common::GraphicsContext* gc);                                                                         //绘画接口
            virtual void OnMouseDown(common::int32_t x, common::int32_t y, common::uint8_t button);                                 //鼠标按下事件接口
            virtual void OnMouseUp(common::int32_t x, common::int32_t y, common::uint8_t button);                                   //鼠标抬起事件接口
            virtual void OnMouseMove(common::int32_t x, common::int32_t y, common::int32_t nx, common::int32_t ny);                 //鼠标移动事件接口
        protected:
            Widget* parent;
            common::int32_t x, y, w, h;
            common::uint8_t r, g, b;
            bool Focussable;                                           //可聚焦
        };
        //复合小部件
        class CompositeWidget : public Widget {
        public:
            CompositeWidget(Widget* parent, common::int32_t x, common::int32_t y,
                common::int32_t w, common::int32_t h, common::uint8_t r, 
                common::uint8_t g, common::uint8_t b);
            ~CompositeWidget();

            virtual void GetFocus(Widget* widget);                                           //获取焦点接口
            virtual bool AddChild(Widget* child);                                            //添加子类接口

            virtual void Draw(common::GraphicsContext* gc) override;                         //绘画接口
            virtual void OnMouseDown(common::int32_t x, common::int32_t y, common::uint8_t button) override;                          //鼠标按下事件接口
            virtual void OnMouseUp(common::int32_t x, common::int32_t y, common::uint8_t button) override;                            //鼠标抬起事件接口
            virtual void OnMouseMove(common::int32_t x, common::int32_t y, common::int32_t nx, common::int32_t ny) override;          //鼠标移动事件接口

            virtual void OnKeyDown(char x) override;                        //键盘按下事件接口
            virtual void OnKeyUp(char x) override;                          //键盘抬起事件接口
        private:
            Widget* children[100];
            common::int32_t numClidren;
            Widget* focussedChild;
        };
    }
}

#endif 