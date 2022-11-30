#include <iostream>
#include <bitset>
#include <fstream>
#include <climits>
#include <memory>

#include "start.h"
//#include "CommandList.h"

std::shared_ptr<CPU> sp_cpu(nullptr);
std::shared_ptr<Mem> sp_mem(nullptr);

CPU* p_cpu;
Mem* p_mem;
bool running = true;

void Run()
{

    //datatypes test section
    /*std::cout << "Size of Byte = " << sizeof(Byte) << std::endl;
    std::cout << "Size of Halfword = " << sizeof(Halfword) << std::endl;
    std::cout << "Size of Word = " << sizeof(Word) << std::endl;*/

    std::fstream sssss; //("ext_memory.sus", std::fstream::trunc | std::fstream::binary);


    /*long address = 1, tempAddr;
    Byte data = 'a', tempData;
    sssss.open("ext_memory.sus", std::fstream::out | std::fstream::trunc | std::fstream::binary);
    sssss.write((char*)&address,sizeof(address));
    sssss.write((char*)&data, sizeof(data));
    sssss.close();*/


    //Init();

    /*Command testCommand{};
    float startNum = 10.0f;
    auto number = reinterpret_cast<unsigned char*>(&startNum);
    std::cout << "2345.5 == ";
    std::bitset<32> bits;
    for (int i = 0; i < sizeof(startNum - 1); ++i) {
        auto temp = std::bitset<8>(number[i]);
        for (int j = 0; j < 8; ++j) {
            if (temp[j] == 1) bits.set(8 * i + j);
        }
    }
    std::cout << "res1 = " << bits << std::endl;
    mem.registers[1] = bits.to_ulong();

    startNum = 5.54f;
    number = reinterpret_cast<unsigned char*>(&startNum);
    std::cout << "2345.5 == ";
    bits = 0;
    for (int i = 0; i < sizeof(startNum - 1); ++i) {
        auto temp = std::bitset<8>(number[i]);
        for (int j = 0; j < 8; ++j) {
            if (temp[j] == 1) bits.set(8 * i + j);
        }
    }
    std::cout << "res2 = " << bits << std::endl;
    mem.registers[2] = bits.to_ulong();

    mem.registers[1] = INT_MAX;
    mem.registers[2] = 1;
    std::cout << mem.registers[1] << std::endl;
    std::cout << mem.registers[2] << std::endl;
    testCommand.opcode = ADD;
    testCommand.param1 = 0;
    testCommand.param2 = 1;
    testCommand.offset = 2;
    testCommand.Execute(mem);*/

    while (running)
    {
        p_cpu->Execute(1, *p_mem);
        p_cpu->PC = 0;
    }

    p_cpu->PC = 42069;

    //std::cout << "res = " << std::bitset<8>(mem.externMem[0]) << ' ' << std::bitset<8>(mem.externMem[1])  << ' ' << std::bitset<8>(mem.externMem[2])  << ' ' << std::bitset<8>(mem.externMem[3]) << std::endl;
    //std::cout << "getfrommem = " << std::bitset<32>(mem.GetWordFromMem(0)) << std::endl;



    /*sssss.open("ext_memory.sus", std::fstream::in | std::fstream::binary);
    sssss.read(reinterpret_cast<char*>(&tempAddr), sizeof(tempAddr));
    sssss.read(reinterpret_cast<char*>(&tempData), sizeof(tempData));
    std::cout << "addr = " << tempAddr << ", data = " << tempData << '\n';*/
	
}

void Init(const std::shared_ptr<CPU> cpu_p, std::shared_ptr<Mem> mem_p)
{
    cpu_p->Reset(*mem_p);
    auto tmp = &*cpu_p;
    Command::SetCpu((&*cpu_p));
}

//Word* GetPcPointer(CPU& cpu)
//{
//    return p_cpu->GetPcPointer();
//}
//
//Word* GetPciPointer(CPU& cpu)
//{
//    return p_cpu->GetPciPointer();
//}
//
//Byte* GetFlagsPointer(CPU& cpu)
//{
//    return p_cpu->GetFlagsPointer();
//}
//
//Byte* GetFlagsiPointer(CPU& cpu)
//{
//    return p_cpu->GetFlagsiPointer();
//}
//
//Word* GetSysregPointer(CPU& cpu)
//{
//    return p_cpu->GetSysregPointer();
//}

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

void TurnOff()
{

}
