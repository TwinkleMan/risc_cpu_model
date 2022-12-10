#include <iostream>
#include <bitset>
#include <fstream>
#include <climits>
#include <memory>

#include "start.h"

std::shared_ptr<CPU> sp_cpu(nullptr);
std::shared_ptr<Mem> sp_mem(nullptr);

CPU* p_cpu;
Mem* p_mem;
bool running = true;

void Run()
{
    while (running)
    {
        p_cpu->Execute(1, *p_mem);
        p_cpu->PC = 0;
    }

    p_cpu->PC = 42069;
	
}

void Init(const std::shared_ptr<CPU> cpu_p, std::shared_ptr<Mem> mem_p)
{
    cpu_p->Reset(*mem_p);
    auto tmp = &*cpu_p;
    Command::SetCpu((&*cpu_p));
}

Word* GetPcPointer()
{
    return p_cpu->GetPcPointer();
}

Word* GetPciPointer()
{
    return p_cpu->GetPciPointer();
}

Byte* GetFlagsPointer()
{
    return p_cpu->GetFlagsPointer();
}

Byte* GetFlagsiPointer()
{
    return p_cpu->GetFlagsiPointer();
}

Word* GetSysregPointer()
{
    return p_cpu->GetSysregPointer();
}

Word* GetRegistersPointer()
{
    return p_cpu->_memory->registers;
}

std::map<long, Byte>* GetExternMemPointer()
{
    return &p_cpu->_memory->externMem;
}
