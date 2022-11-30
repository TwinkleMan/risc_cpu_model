#pragma once
#include "Command.h"

/* flags
| 0 | 1 | 2 | 3 | 4 | 5 | 6 |
| Z | C | S | O | I | T | U |

 Z - zero result
 C - high-order carryover
 S - result sign
 O - result overflow
 I - interrupt enable
 T - step-by-step mode
 U - root mode
*/
struct CPU {
    Word PC;        //program counter
    Word PCI;       //Interrupt program counter (temporary storage for regular PC)
    Byte FLAGS;     //System flags
    Byte FLAGSI;    //Interrupt flags (temporary storage for regular flags)
    Word TLB;       //Association translation buffer
    Word SYSREG;    //System register

    int interruptCode;
    int stepMode;
    Mem* _memory;

    Word* GetPcPointer();
    Word* GetPciPointer();
    Byte* GetFlagsPointer();
    Byte* GetFlagsiPointer();
    Word* GetSysregPointer();


    void Reset(Mem& memory);

    Byte FetchByte(u32& cycles, Mem& memory);

    /*fetch word from a extern_memory_file, get command from extern_memory_file*/
    Word FetchWord(u32& cycles, Mem& memory);

    /*translate fetched command*/
    Command TranslateCommand(u32& cycles, Word data);

    /*execute command, constructed in TranslateCommand*/
    void RunCommand(Command& command, Mem& memory);

    std::string HandleInterruptions();

    void Execute(u32 cycles, Mem& memory);
    std::string Execute(Command& command, Mem& memory);

    void SetZFlag();
    void SetCFlag();
    void SetSFlag();
    void SetOFlag();
    void SetIFlag();
    void SetTFlag();
    void SetUFlag();

    void ResetZFlag();
    void ResetCFlag();
    void ResetSFlag();
    void ResetOFlag();
    void ResetIFlag();
    void ResetTFlag();
    void ResetUFlag();

    Word GetPC();
};
