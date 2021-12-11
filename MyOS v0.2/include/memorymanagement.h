#ifndef __MYOS__MEMORYMANAGEMENT_
#define __MYOS__MEMORYMANAGEMENT_
/**********内存管理**********/
#include "common/types.h"

namespace myos {
    //内存块
    struct MemoryChunk {
        MemoryChunk* next;
        MemoryChunk* prev;
        bool allocated;          //分配
        common::size_t size;
    };
    //内存管理
    class MemoryManager {
    public:
        MemoryManager(common::size_t first, common::size_t size);
        ~MemoryManager();

        void* malloc(common::size_t size);         //堆
        void free(void* ptr);                      //释放

        static MemoryManager* activeMemoryManager; //激活内存管理
    private:
        MemoryChunk* first;
    };
}
//重载运算符
void* operator new(myos::common::size_t size);
void* operator new[](myos::common::size_t size);

void* operator new(myos::common::size_t size, void* ptr);
void* operator new[](myos::common::size_t size, void* ptr);

void operator delete(void* ptr);
void operator delete[](void* ptr);

#endif