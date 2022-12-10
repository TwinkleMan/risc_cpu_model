#pragma once
#include <memory>

#include "cpu.h"

void Run();
void Init(std::shared_ptr<CPU> cpu_p, std::shared_ptr<Mem> mem_p);

Word* GetPcPointer();
Word* GetPciPointer();
Byte* GetFlagsPointer();
Byte* GetFlagsiPointer();
Word* GetSysregPointer();
Word* GetRegistersPointer();
std::map<long, Byte>* GetExternMemPointer();
