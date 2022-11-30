#pragma once
#include "Memory.h"

static constexpr Byte

        //fixed point arithmetics
        ADD = 0x01,
        ADDU = 0x02,
        SUB = 0x03,
        SUBU = 0x04,
        ADDI = 0x05,
        SLT = 0x06,
        SLTI = 0x07,
        SLTU = 0x08,
        SLTIU = 0x09,
        LUI = 0x0A,     //load upper immediate (rd <- imm19 << 13)
        AUIP = 0x0B,
        MUL = 0x0C,
        MULU = 0x0D,
        DIV = 0x0E,
        DIVU = 0x0F,

        //floating point arithmetics
        ADDF = 0x11,
        SUBF = 0x12,
        MULF = 0x13,
        DIVF = 0x14,
        TFIX = 0x15,
        TFLT = 0x16,

        //logical operations
        AND = 0x17,     //+
        OR = 0x18,      //+
        XOR = 0x19,     //+
        SLL = 0x1A,
        SRL = 0x1B,
        SRA = 0x1C,


        //load/store operations
        LW = 0x1D,        //+
        LH = 0x1E,      //+
        LB = 0x1F,      //+
        SW = 0x20,      //+
        SH = 0x21,      //+
        SB = 0x22,      //+
        MOV = 0x23,
        SWP = 0x24,
		IN = 0x25,
		OUT = 0x26,


		//command flow control
		CLI = 0x27,
		STI = 0x28,
		INT = 0x29,
		IRET = 0x2A,
		CALL = 0x2B,
		RET = 0x2C,
		JMP = 0x2D,
		JZ = 0x2E,
		JNZ = 0x2F,
		JO = 0x30,
		JNO = 0x31,
		JC = 0x32,
		JNC = 0x33,
		JS = 0x34,
		JNS = 0x35,

		
        //Special commands
		NOP = 0x36,
		RFL = 0x37,
		WFL = 0x38,
		RTLB = 0x39,
		WTLB = 0x3A,
		RFE = 0x3B,
		SCALL = 0x3C,


        TEST = 222;



/*|  opcode  |   R0   |   R1   | offset |
 *| 31....25 | 24..19 | 18..13 | 12...0 |
 *   7 bit     6 bit    6 bit    13 bit
 */
struct Command {
    Byte opcode;
    Byte param1, param2;
    Halfword offset;
    Word* currentPC;

    inline static void* _cpu = nullptr;

    static void SetCpu(void* p_cpu);
    void Execute(Mem& memory);
    void Clear();
};

Command BuildCommand(const char* data);

Byte GetInfoFromText(std::string& operation);
