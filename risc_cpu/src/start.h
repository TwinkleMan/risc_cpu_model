#pragma once
#include "cpu.h"

#include <memory>

void Run();
void Init(std::shared_ptr<CPU> cpu_p, std::shared_ptr<Mem> mem_p);

//Word* GetPcPointer(CPU& cpu);
//Word* GetPciPointer(CPU& cpu);
//Byte* GetFlagsPointer(CPU& cpu);
//Byte* GetFlagsiPointer(CPU& cpu);
//Word* GetSysregPointer(CPU& cpu);

Word* GetPcPointer();
Word* GetPciPointer();
Byte* GetFlagsPointer();
Byte* GetFlagsiPointer();
Word* GetSysregPointer();
Word* GetRegistersPointer();
std::map<long, Byte>* GetExternMemPointer();

void TurnOff();
