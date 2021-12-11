
#ifndef __MYOS__MULTITASING_H
#define __MYOS__MULTITASING_H
/**********多任务处理**********/
#include "common/types.h"
#include "gdt.h"

namespace myos {
    //CPU状态
    struct CPUState {
        common::uint32_t eax, ebx, ecx, edx, esi, edi, ebp;

        common::uint32_t error, eip, cs, eflags, esp, ss;
    } __attribute__((packed));
    //任务
    class Task {
        friend class TaskManger;    //任务管理
    public:
        Task(GlobalDescriptorTable* gdt, void entrypoint());
        ~Task();
    private:
        common::uint8_t stack[4096];
        CPUState* cpustate;
    };
    //任务管理
    class TaskManger {
    public:
        TaskManger();
        ~TaskManger();
        bool AddTask(Task* task);    //添加任务
        CPUState* Schedule(CPUState* cpustate);
    private:
        Task* tasks[256];
        int numTasks;
        int currentTask;    //任务流
    };
}

#endif