#ifndef __MYOS__DRIVERS__DRIVER_H
#define __MYOS__DRIVERS__DRIVER_H
/**********驱动**********/
namespace myos 
{
    namespace drivers 
    {   //驱动类
        class Driver {
        public:
            Driver(); 
            ~Driver();
            // 定义接口
            virtual void Activate();      //启动接口
            virtual int Reset();          //重启接口
            virtual void Deactivate();    //停用接口
        };
        //驱动管理类
        class DriverManager {
        public:
            DriverManager();
            void AddDriver(Driver* );     //添加驱动
            void ActivateAll();           // 激活所有驱动

            Driver* drivers[256];         
            int numDrivers;               //驱动数量
        };
    }
}


#endif 