#include <vector>

#include "Memory.h"

void Mem::Initialise() {
    for (Word &iter : registers) {
        iter = 0;
    }

    //interruptTable[0] = MemoryInterrupt;

    interruptTable.emplace(0,&Mem::MemoryInterrupt);
    interruptTable.emplace(1,&Mem::VideoInterrupt);
    interruptTable.emplace(2,&Mem::PageException);
    interruptTable.emplace(3,&Mem::CommandException);
    interruptTable.emplace(4,&Mem::StepModeException);
    interruptTable.emplace(5,&Mem::UserException);

    InitialiseExtern(100);
    //SetHalfwordToMem(69, 32000);

}

void Mem::TestInit() {
    u32 temp = 0;

    Initialise();

    for (Word &iter : registers) {
        if (temp == 0) {
            Word tempWord;
            //tempWord = 0xFE000000;    //11111110000000000000000000000000
            //tempWord = 1 << 31 | 1 << 25 | 1 << 24 | 1 << 19 | 1 << 18 | 1 << 13 | 1 << 12 | 1 << 0;

            //put BYTE 5 to extern_memory_file
            /*tempWord = 1 << 29 | 1 << 28 | 1 << 26 | 1 << 25;       //opcode
            tempWord |= 1 << 19;                                    //param1
            tempWord |= 1 << 18 | 1 << 13;                          //param2
            tempWord |= 1 << 2 | 1 << 0;*/                           //offset

            //put HALFWORD 65000 to extern_memory_file
            /*tempWord = 1 << 29 | 1 << 28 | 1 << 27;       //opcode
            tempWord |= 1 << 19;                                    //param1
            tempWord |= 1 << 18 | 1 << 17 | 1 << 16 | 1 << 15 | 1 << 14 | 1 << 13;                          //param2
            tempWord |= 1 << 12 | 1 << 11 | 1 << 10 | 1 << 9 | 1 << 8 | 1 << 7 | 1 << 6 | 1 << 5 | 1 << 4 | 1 << 3;  //offset*/

            //ADDI test
            // 00000110000010000001110101010101
            /*tempWord = 1 << 26 | 1 << 25;
            tempWord |= 1 << 19;
            tempWord |= 1 << 12 | 1 << 11 | 11 << 10 | 1 << 8 | 1 << 6 | 1 << 4 | 1 << 2 | 1 << 0;
            tempWord = 0x6081D55;*/

            tempWord = 1 << 27;
            tempWord |= 1 << 19;
            tempWord |= 1 << 14;
            tempWord |= 1 << 1 | 1 << 0;

            iter = tempWord;
            temp++;
        } else iter = 0;
    }

    SetHalfwordToMem(69, 32000);
    SetWordToMem(0,registers[0]);

    registers[2] = 127;
    registers[3] = 7;
    registers[4] = 32000;
    registers[5] = 255;
    registers[6] |= 0x22;
}





void Mem::InitialiseExtern(long size) {
    for (int i = 0; i < size; ++i) {
        externMem[i] = 0;
    }
    for (auto it: externMem) {
        SetByteToMem(it.first, it.second);
    }
}

void Mem::SetByteToMem(long address, Byte data) {
    long tempAddr;
    Byte tempData;
    std::fstream file;

    file.open("ext_memory.sus", std::fstream::in |  std::fstream::ate | std::fstream::binary);
    int size = file.tellg();
    file.seekg(0);

    int i = 0;
    if (size >= 2) {
        for (; !file.eof(); ++i) {
            file.read(reinterpret_cast<char *>(&tempAddr), sizeof(tempAddr));
            file.read(reinterpret_cast<char *>(&tempData), sizeof(tempData));
            externMem[tempAddr] = tempData;
        }
        file.close();
    }

    externMem[address] = data;

    file.open("ext_memory.sus", std::fstream::out | std::fstream::trunc | std::fstream::binary);
    for (auto & it : externMem) {
        if (it.first == address) {
            file.write((char*)&it.first,sizeof(address));
            file.write((char*)&it.second, sizeof(data));
            break;
        }
    }
    file.close();
}


void Mem::SetHalfwordToMem(long address, Halfword data) {
    Byte dataHigher, dataLower;

    dataHigher = data >> 8;
    dataLower = data;

    long tempAddr;
    Byte tempData;
    std::fstream file;

    file.open("ext_memory.sus", std::fstream::in |  std::fstream::app | std::fstream::binary);
    int size = file.tellg();
    file.seekg(0);

    int i = 0;
    if (size >= 2) {
        for (; !file.eof(); ++i) {
            file.read(reinterpret_cast<char *>(&tempAddr), sizeof(tempAddr));
            file.read(reinterpret_cast<char *>(&tempData), sizeof(tempData));
            externMem[tempAddr] = tempData;
        }
        file.close();
    }

    externMem[address] = dataHigher;
    externMem[address+1] = dataLower;

    file.open("ext_memory.sus", std::fstream::out | std::fstream::trunc | std::fstream::binary);
    for (auto & it : externMem) {
        if (it.first == address || it.first == (address + 1)) {
            file.write((char *) &it.first, sizeof(address));
            file.write((char *) &it.second, sizeof(data));
            if (it.first == (address + 1)) break;
        }
    }
    file.close();
}

void Mem::SetWordToMem(long address, Word data) {
    Byte dataFirst, dataSecond, dataThird, dataFourth;

    dataFirst = data >> 24;
    dataSecond = data >> 16;
    dataThird = data >> 8;
    dataFourth = data;

    long tempAddr;
    Byte tempData;
    std::fstream file;

    file.open("ext_memory.sus", std::fstream::in |  std::fstream::app | std::fstream::binary);
    int size = file.tellg();
    file.seekg(0);

    int i = 0;
    if (size >= 1) {
        for (; !file.eof(); ++i) {
            file.read(reinterpret_cast<char *>(&tempAddr), sizeof(tempAddr));
            file.read(reinterpret_cast<char *>(&tempData), sizeof(tempData));
            externMem[tempAddr] = tempData;
        }
        file.close();
    }

    externMem[address] = dataFirst;
    externMem[address+1] = dataSecond;
    externMem[address+2] = dataThird;
    externMem[address+3] = dataFourth;

    file.open("ext_memory.sus", std::fstream::out | std::fstream::trunc | std::fstream::binary);
    for (auto & it : externMem) {
        if (it.first >= address || it.first <= address + 3) {
            file.write((char*)&it.first,sizeof(address));
            file.write((char*)&it.second, sizeof(data));
            if (it.first == (address + 3)) break;
        }
    }
    file.close();
}

Byte Mem::GetByteFromMem(long address) {
    Byte result;

    long tempAddr;
    Byte tempData;
    std::fstream file;

    file.open("ext_memory.sus", std::fstream::in |  std::fstream::app | std::fstream::binary);
    file.seekg(0);

    int i = 0;
    for (;!file.eof();++i) {
        file.read(reinterpret_cast<char*>(&tempAddr), sizeof(tempAddr));
        file.read(reinterpret_cast<char*>(&tempData), sizeof(tempData));
        externMem[tempAddr] = tempData;
    }
    file.close();

    result = externMem[address];

    return result;
}

Halfword Mem::GetHalfwordFromMem(long address) {
    Halfword result;

    long tempAddr;
    Byte tempData;
    std::fstream file;

    file.open("ext_memory.sus", std::fstream::in |  std::fstream::app | std::fstream::binary);
    file.seekg(0);

    int i = 0;
    for (;!file.eof();++i) {
        file.read(reinterpret_cast<char*>(&tempAddr), sizeof(tempAddr));
        file.read(reinterpret_cast<char*>(&tempData), sizeof(tempData));
        externMem[tempAddr] = tempData;
    }
    file.close();

    result = externMem[address] << 8;
    result |= externMem[address + 1];

    return result;
}

Word Mem::GetWordFromMem(long address) {
    Word result;

    long tempAddr;
    Byte tempData;
    std::fstream file;

    file.open("ext_memory.sus", std::fstream::in |  std::fstream::app | std::fstream::binary);
    file.seekg(0);

    int i = 0;
    for (;!file.eof();++i) {
        file.read(reinterpret_cast<char*>(&tempAddr), sizeof(tempAddr));
        file.read(reinterpret_cast<char*>(&tempData), sizeof(tempData));
        externMem[tempAddr] = tempData;
    }
    file.close();

    result = externMem[address] << 24;
    result |= externMem[address + 1] << 16;
    result |= externMem[address + 2] << 8;
    result |= externMem[address + 3];

    return result;
}



//interruption handlers
std::string Mem::MemoryInterrupt(void)
{
    return std::string("Memory handler done!");
}

std::string Mem::VideoInterrupt(void)
{
    return std::string("Video handler done!");
}

std::string Mem::PageException(void)
{
    return std::string("Page exception!!!");
}

std::string Mem::CommandException(void)
{
    return std::string("Command execution exception!!!");
}

std::string Mem::StepModeException(void)
{
    return std::string("Step done;");
}

std::string Mem::UserException(void)
{
    return std::string("User defined exception done");
}
