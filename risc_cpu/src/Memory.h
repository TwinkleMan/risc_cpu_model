#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <map>

using Byte = unsigned char;         //8
using Halfword = unsigned short;    //16
using Word = unsigned int;          //32

using u32 = unsigned int;

struct Mem {
    static const Byte MAX_MEM = 64;
    Word registers[MAX_MEM];
    std::map<long, Byte> externMem;
    Word port[MAX_MEM];

    //interrupt table
    //typedef std::string(Mem::* ScriptFunction)();
    using ScriptFunction = std::string(Mem::*)();
    std::map<int, ScriptFunction> interruptTable;

    void Initialise();
    void InitialiseExtern(long size);
    void TestInit();
    Byte GetByteFromMem(long address);
    Halfword GetHalfwordFromMem(long address);
    Word GetWordFromMem(long address);

    void SetByteToMem(long address, Byte data);
    void SetHalfwordToMem(long address, Halfword data);
    void SetWordToMem(long address, Word data);

    std::string MemoryInterrupt();
    std::string VideoInterrupt();
    std::string PageException();
    std::string CommandException();
    std::string StepModeException();
    std::string UserException();

    Word* GetRegistersPointer() { return registers; }
    std::map<long, Byte>* GetExternMapPointer() { return &externMem; }

    Word operator[](u32 address) const {
        //assert that address is < MAX_MEM
        return registers[address];
    }
};
