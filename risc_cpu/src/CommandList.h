#pragma once

#include <bitset>
#include <cstring>
#include <float.h>

#include "cpu.h"

namespace Commands {

    CPU* _cpu = nullptr;
    //std::shared_ptr<Mem> _mem = nullptr;

    //misc
    template <typename T>
    int convertToSigned(T input) {
        int result;
        int size = sizeof(T) * 8 - 1;
        auto bits = std::bitset<32>(input);
        bool first = bits[size];

        if (first) {
            for (int i = 0; i < size; ++i) {
                bits ^= (1 << i);
            }
            bits[size] = false;
            result = bits.to_ulong();
            result += 1;
            result = -result;
        }
        else {
            result = bits.to_ulong();
        }


        std::cout << "Input = " << std::bitset<32>(input) << ", result = " << result << std::endl;

        return result;
    }

    void SetCpu(CPU* cpu) {
        _cpu = cpu;
    }


    /*ADD    rd, rs1, rs2*/
    /*rd <- rs1 + rs2*/
    void ADD(Mem& memory, Command command) {
        int first = convertToSigned(memory.registers[command.param2]);
        int second = convertToSigned(memory.registers[command.offset]);
        long long result = static_cast<long long>(first) + static_cast<long long>(second);

        if (result == 0) _cpu->SetZFlag();
        if (result > INT32_MAX || result < INT32_MIN) {
            _cpu->SetOFlag();
            _cpu->SetCFlag();
        }
        if (result & (1 << 31)) _cpu->SetSFlag();
        else _cpu->ResetSFlag();

        memory.registers[command.param1] = result;
        std::cout << "ADD rd = " << std::bitset<32>(memory.registers[command.param1]) << ": " << result << std::endl;
    }

    /*ADDU rd, rs1, rs2*/
    /*rd <- rs1 + rs2*/
    void ADDU(Mem& memory, Command command) {
        int first = memory.registers[command.param2];
        int second = memory.registers[command.offset];
        long long result = static_cast<long long>(first) + static_cast<long long>(second);

        if (result == 0) _cpu->SetZFlag();
        if (result > INT32_MAX || result < INT32_MIN) {
            _cpu->SetOFlag();
            _cpu->SetCFlag();
        }
        if (result & (1 << 31)) _cpu->SetSFlag();
        else _cpu->ResetSFlag();

        memory.registers[command.param1] = memory.registers[command.param2] + memory.registers[command.offset];
        std::cout << "ADDU rd = " << std::bitset<32>(memory.registers[command.param1]) << std::endl;
    }

    /*SUB rd, rs1, rs2*/
    /*rd <- rs1 - rs2*/
    void SUB(Mem& memory, Command command) {
        int first = convertToSigned(memory.registers[command.param2]);
        int second = convertToSigned(memory.registers[command.offset]);
        long long result = static_cast<long long>(first) - static_cast<long long>(second);

        if (result == 0) _cpu->SetZFlag();
        if (result > INT32_MAX || result < INT32_MIN) {
            _cpu->SetOFlag();
            _cpu->SetCFlag();
        }
        if (result & (1 << 31)) _cpu->SetSFlag();
        else _cpu->ResetSFlag();

        memory.registers[command.param1] = first - second;
        std::cout << "ADD rd = " << std::bitset<32>(memory.registers[command.param1]) << ": " << first - second << std::endl;
    }

    /*SUBU rd, rs1, rs2*/
    /*rd <- rs1 - rs2*/
    void SUBU(Mem& memory, Command command) {
        int first = memory.registers[command.param2];
        int second = memory.registers[command.offset];
        long long result = static_cast<long long>(first) - static_cast<long long>(second);

        if (result == 0) _cpu->SetZFlag();
        if (result > INT32_MAX || result < INT32_MIN) {
            _cpu->SetOFlag();
            _cpu->SetCFlag();
        }
        if (result & (1 << 31)) _cpu->SetSFlag();
        else _cpu->ResetSFlag();

        memory.registers[command.param1] = memory.registers[command.param2] - memory.registers[command.offset];
        std::cout << "SUBU rd = " << std::bitset<32>(memory.registers[command.param1]) << std::endl;
    }

    /*ADDI rd, rs1, imm13*/
    /*rd <- rs1 + imm13*/
    void ADDI(Mem& memory, Command command) {
        int first = convertToSigned(memory.registers[command.param2]);
        int second = convertToSigned(command.offset);
        long long result = static_cast<long long>(first) + static_cast<long long>(second);

        if (result == 0) _cpu->SetZFlag();
        if (result > INT32_MAX || result < INT32_MIN) {
            _cpu->SetOFlag();
            _cpu->SetCFlag();
        }
        if (result & (1 << 31)) _cpu->SetSFlag();
        else _cpu->ResetSFlag();

        memory.registers[command.param1] = memory.registers[command.param2] + command.offset;
        std::cout << "ADDI rd = " << std::bitset<32>(memory.registers[command.param1]) << std::endl;
    }

    /*SLT rd, rs1, rs2*/
    /*rd <- rs1 < rs2 ? 1 : 0*/
    void SLT(Mem& memory, Command command) {
        Byte negativeFlagFirst = 0;
        Byte negativeFlagSecond = 0;

        std::cout << "rs1 = " << std::bitset<32>(memory.registers[command.param2]) << std::endl;
        std::cout << "rs2 = " << std::bitset<32>(memory.registers[command.offset]) << std::endl;
			
        Word op1 = 0, op2 = 0;
        if (memory.registers[command.param2] >> 31 & 1) {
            //negative number
            negativeFlagFirst = 1;
            op1 = memory.registers[command.param2] - 1;
            for (int i = 0; i < 32; ++i) {
                op1 ^= (1 << i);
            }
        } else op1 = memory.registers[command.param2];

        if (memory.registers[command.offset] >> 31 & 1) {
            //negative number
            negativeFlagSecond = 1;
            op2 = memory.registers[command.offset] - 1;
            for (int i = 0; i < 32; ++i) {
                op2 ^= (1 << i);
            }
        } else op2 = memory.registers[command.offset];

        if (negativeFlagFirst == 1 && negativeFlagSecond == 0) {
            memory.registers[command.param1] = 1;
        } else if (negativeFlagFirst == 0 && negativeFlagSecond == 1) {
            memory.registers[command.param1] = 0;
        } else if (negativeFlagFirst == 1 && negativeFlagSecond == 1) {
            op1 > op2 ? memory.registers[command.param1] = 1 : memory.registers[command.param1] = 0;
        } else {
            op1 < op2 ? memory.registers[command.param1] = 1 : memory.registers[command.param1] = 0;
        }

        std::cout << "SLT rd = " << std::bitset<32>(memory.registers[command.param1]) << std::endl;
    }

    /*SLTI rd, rs1, imm13*/
    /*rd <- rs1 < rs2 ? 1 : 0*/
    void SLTI(Mem& memory, Command command) {
        Byte negativeFlagFirst = 0;
        Byte negativeFlagSecond = 0;

        std::cout << "rs1 = " << std::bitset<32>(memory.registers[command.param2]) << std::endl;
        std::cout << "rs2 = " << std::bitset<32>(memory.registers[command.offset]) << std::endl;

        Word op1 = 0, op2 = 0;
        if (memory.registers[command.param2] >> 31 & 1) {
            //negative number
            negativeFlagFirst = 1;
            op1 = memory.registers[command.param2] - 1;
            for (int i = 0; i < 32; ++i) {
                op1 ^= (1 << i);
            }
        } else op1 = memory.registers[command.param2];

        if (command.offset >> 12 & 1) {
            //negative number
            negativeFlagSecond = 1;
            op2 = command.offset - 1;
            for (int i = 0; i < 13; ++i) {
                op2 ^= (1 << i);
            }
        } else op2 = command.offset;

        if (negativeFlagFirst == 1 && negativeFlagSecond == 0) {
            memory.registers[command.param1] = 1;
        } else if (negativeFlagFirst == 0 && negativeFlagSecond == 1) {
            memory.registers[command.param1] = 0;
        } else if (negativeFlagFirst == 1 && negativeFlagSecond == 1) {
            op1 > op2 ? memory.registers[command.param1] = 1 : memory.registers[command.param1] = 0;
        } else {
            op1 < op2 ? memory.registers[command.param1] = 1 : memory.registers[command.param1] = 0;
        }

        std::cout << "SLTI rd = " << std::bitset<32>(memory.registers[command.param1]) << std::endl;
    }

    /*SLTU rd, rs1, rs2*/
    /*rd <- rs1 < rs2 ? 1 : 0*/
    void SLTU(Mem& memory, Command command) {
        memory.registers[command.param2] < memory.registers[command.offset] ? memory.registers[command.param1] = 1 : memory.registers[command.param1] = 0;
        std::cout << "SLTU rd = " << std::bitset<32>(memory.registers[command.param1]) << std::endl;
    }

    /*SLTIU rd, rs1, imm13*/
    /*rd <- rs1 < imm13 ? 1 : 0*/
    void SLTIU(Mem& memory, Command command) {
        memory.registers[command.param2] < command.offset ? memory.registers[command.param1] = 1 : memory.registers[command.param1] = 0;
        std::cout << "SLTIU rd = " << std::bitset<32>(memory.registers[command.param1]) << std::endl;
    }

    /*LUI rd, imm19*/
    /*rd <- imm19 << 13*/
    void LUI(Mem& memory, Command command) {
        memory.registers[command.param1] = command.offset << 13;

        if (memory.registers[command.param1] == 0) _cpu->SetZFlag();
        if (memory.registers[command.param1] & (1 << 31)) _cpu->SetSFlag();
        else _cpu->ResetSFlag();

        std::cout << "LUI rd = " << std::bitset<32>(memory.registers[command.param1]) << std::endl;
    }

    /*AUIP rd, imm19*/
    /*rd <- PC + imm19 << 13*/
    void AUIP(Mem& memory, Command command) {
        memory.registers[command.param1] = *(command.currentPC) + (command.offset << 13);
        long long result = 
            static_cast<long long>(*(command.currentPC)) + static_cast<long long>(command.offset << 13);

    	if (result == 0) _cpu->SetZFlag();
        if (result > INT32_MAX || result < INT32_MIN) {
            _cpu->SetOFlag();
            _cpu->SetCFlag();
        }
        if (result & (1 << 31)) _cpu->SetSFlag();
        else _cpu->ResetSFlag();

        std::cout << "AUIP rd = " << std::bitset<32>(memory.registers[command.param1]) << std::endl;
    }

    /*MUL rd, rs1, rs2*/
    /*rd <- rs1 * rs2*/
    void MUL(Mem& memory, Command command) {
        int first = convertToSigned(memory.registers[command.param2]);
        int second = convertToSigned(memory.registers[command.offset]);
        long long result = static_cast<long long>(first) * static_cast<long long>(second);

        if (result == 0) _cpu->SetZFlag();
        if (result > INT32_MAX || result < INT32_MIN) {
            _cpu->SetOFlag();
            _cpu->SetCFlag();
        }
        if (result & (1 << 31)) _cpu->SetSFlag();
        else _cpu->ResetSFlag();

        memory.registers[command.param1] = first * second;
        std::cout << "MUL rd = " << std::bitset<32>(memory.registers[command.param1]) << ": " << first * second << std::endl;
    }

    /*MULU rd, rs1, rs2*/
    /*rd <- rs1 * rs2*/
    void MULU(Mem& memory, Command command) {
        int first = memory.registers[command.param2];
        int second = memory.registers[command.offset];
        long long result = static_cast<long long>(first) * static_cast<long long>(second);

        if (result == 0) _cpu->SetZFlag();
        if (result > INT32_MAX || result < INT32_MIN) {
            _cpu->SetOFlag();
            _cpu->SetCFlag();
        }
        if (result & (1 << 31)) _cpu->SetSFlag();
        else _cpu->ResetSFlag();

        memory.registers[command.param1] = memory.registers[command.param2] * memory.registers[command.offset];
        std::cout << "MULU rd = " << std::bitset<32>(memory.registers[command.param1]) << ": " << memory.registers[command.param2] * memory.registers[command.offset] << std::endl;
    }

    /*DIV rd, rs1, rs2*/
    /*rd <- rs1 / rs2*/
    void DIV(Mem& memory, Command command) {
        int first = convertToSigned(memory.registers[command.param2]);
        int second = convertToSigned(memory.registers[command.offset]);
        long long result = static_cast<long long>(first) / static_cast<long long>(second);

        if (result == 0) _cpu->SetZFlag();
        if (result > INT32_MAX || result < INT32_MIN) {
            _cpu->SetOFlag();
            _cpu->SetCFlag();
        }
        if (result & (1 << 31)) _cpu->SetSFlag();
        else _cpu->ResetSFlag();

        memory.registers[command.param1] = first / second;
        std::cout << "DIV rd = " << std::bitset<32>(memory.registers[command.param1]) << ": " << first / second << std::endl;
    }

    /*DIVU rd, rs1, rs2*/
    /*rd <- rs1 / rs2*/
    void DIVU(Mem& memory, Command command) {
        int first = memory.registers[command.param2];
        int second = memory.registers[command.offset];
        long long result = static_cast<long long>(first) / static_cast<long long>(second);

        if (result == 0) _cpu->SetZFlag();
        if (result > INT32_MAX || result < INT32_MIN) {
            _cpu->SetOFlag();
            _cpu->SetCFlag();
        }
        if (result & (1 << 31)) _cpu->SetSFlag();
        else _cpu->ResetSFlag();

        memory.registers[command.param1] = memory.registers[command.param2] / memory.registers[command.offset];
        std::cout << "DIVU rd = " << std::bitset<32>(memory.registers[command.param1]) << ": " << memory.registers[command.param2] / memory.registers[command.offset] << std::endl;
    }



    //floating point arithmetics
    /*ADDF rd, rs1, rs2*/
    /*rd <- rs1 + rs2*/
    void ADDF(Mem& memory, Command command) {
    	float num1 = 0.0f;
        float num2 = 0.0f;
        memcpy(&num1,&memory.registers[command.param2], sizeof(float));
        memcpy(&num2,&memory.registers[command.offset], sizeof(float));

        float result = num1 + num2;
        auto number = reinterpret_cast<unsigned char*>(&result);
        std::bitset<32> bits;
        for (int i = 0; i < sizeof(result - 1); ++i) {
            auto temp = std::bitset<8>(number[i]);
            for (int j = 0; j < 8; ++j) {
                if (temp[j] == 1) bits.set(8 * i + j);
            }
        }
        double test_result = static_cast<double>(num1) + static_cast<double>(num2);
        if (test_result == 0.0f) _cpu->SetZFlag();
        if (test_result > FLT_MAX || test_result < FLT_MIN) {
            _cpu->SetOFlag();
            _cpu->SetCFlag();
        }
        if (bits[31] == 1) _cpu->SetSFlag();
        else _cpu->ResetSFlag();

        memory.registers[command.param1] = bits.to_ulong();

        std::cout << num1 << " + " << num2 << " = " << result << std::endl;
    }

    /*SUBF rd, rs1, rs2*/
    /*rd <- rs1 - rs2*/
    void SUBF(Mem& memory, Command command) {
        float num1 = 0.0f;
        float num2 = 0.0f;
        memcpy(&num1,&memory.registers[command.param2], sizeof(float));
        memcpy(&num2,&memory.registers[command.offset], sizeof(float));

        float result = num1 - num2;
        auto number = reinterpret_cast<unsigned char*>(&result);
        std::bitset<32> bits;
        for (int i = 0; i < sizeof(result - 1); ++i) {
            auto temp = std::bitset<8>(number[i]);
            for (int j = 0; j < 8; ++j) {
                if (temp[j] == 1) bits.set(8 * i + j);
            }
        }
        double test_result = static_cast<double>(num1) - static_cast<double>(num2);
        if (test_result == 0.0f) _cpu->SetZFlag();
        if (test_result > FLT_MAX || test_result < FLT_MIN) {
            _cpu->SetOFlag();
            _cpu->SetCFlag();
        }
        if (bits[31] == 1) _cpu->SetSFlag();
        else _cpu->ResetSFlag();

        memory.registers[command.param1] = bits.to_ulong();

        std::cout << num1 << " - " << num2 << " = " << result << std::endl;
    }

    /*MULF rd, rs1, rs2*/
    /*rd <- rs1 * rs2*/
    void MULF(Mem& memory, Command command) {
        float num1 = 0.0f;
        float num2 = 0.0f;
        memcpy(&num1,&memory.registers[command.param2], sizeof(float));
        memcpy(&num2,&memory.registers[command.offset], sizeof(float));

        float result = num1 * num2;
        auto number = reinterpret_cast<unsigned char*>(&result);
        std::bitset<32> bits;
        for (int i = 0; i < sizeof(result - 1); ++i) {
            auto temp = std::bitset<8>(number[i]);
            for (int j = 0; j < 8; ++j) {
                if (temp[j] == 1) bits.set(8 * i + j);
            }
        }
        double test_result = static_cast<double>(num1) * static_cast<double>(num2);
        if (test_result == 0.0f) _cpu->SetZFlag();
        if (test_result > FLT_MAX || test_result < FLT_MIN) {
            _cpu->SetOFlag();
            _cpu->SetCFlag();
        }
        if (bits[31] == 1) _cpu->SetSFlag();
        else _cpu->ResetSFlag();

        memory.registers[command.param1] = bits.to_ulong();

        std::cout << num1 << " * " << num2 << " = " << result << std::endl;
    }

    /*DIVF rd, rs1, rs2*/
    /*rd <- rs1 / rs2*/
    void DIVF(Mem& memory, Command command) {
        float num1 = 0.0f;
        float num2 = 0.0f;
        memcpy(&num1,&memory.registers[command.param2], sizeof(float));
        memcpy(&num2,&memory.registers[command.offset], sizeof(float));

        float result = num1 / num2;
        auto number = reinterpret_cast<unsigned char*>(&result);
        std::bitset<32> bits;
        for (int i = 0; i < sizeof(result - 1); ++i) {
            auto temp = std::bitset<8>(number[i]);
            for (int j = 0; j < 8; ++j) {
                if (temp[j] == 1) bits.set(8 * i + j);
            }
        }
        double test_result = static_cast<double>(num1) / static_cast<double>(num2);
        if (test_result == 0.0f) _cpu->SetZFlag();
        if (test_result > FLT_MAX || test_result < FLT_MIN) {
            _cpu->SetOFlag();
            _cpu->SetCFlag();
        }
        if (bits[31] == 1) _cpu->SetSFlag();
        else _cpu->ResetSFlag();

        memory.registers[command.param1] = bits.to_ulong();

        std::cout << num1 << " / " << num2 << " = " << result << std::endl;
    }

    /*TFLT rd, rs1*/
    /*rd <- float(rs1)*/
    void TFLT(Mem& memory, Command command) {
        memory.registers[command.param1] = static_cast<float>(memory.registers[command.param2]);

        auto test_result = static_cast<double>(memory.registers[command.param2]);
        if (test_result > FLT_MAX || test_result < FLT_MIN) {
            _cpu->SetOFlag();
            _cpu->SetCFlag();
        }
        if (memory.registers[command.param2] & (1 << 31)) _cpu->SetSFlag();
        else _cpu->ResetSFlag();

        std::cout << std::bitset<32>(memory.registers[command.param1]) << std::endl;

    }

    /*TFIX rd, rs1*/
    /*rd <- int(rs1)*/
    void TFIX(Mem& memory, Command command) {
        float newFloat = 0.0f;
        memcpy(&newFloat,&memory.registers[command.param2], sizeof(float));
        memory.registers[command.param1] = static_cast<int>(newFloat);

        auto test_result = static_cast<long long>(newFloat);
        if (test_result > INT32_MAX || test_result < INT32_MIN) {
            _cpu->SetOFlag();
            _cpu->SetCFlag();
        }
        if (test_result & (1 << 31)) _cpu->SetSFlag();
        else _cpu->ResetSFlag();

        std::cout << std::bitset<32>(memory.registers[command.param1]) << std::endl;
    }



    //logical operations
    /*AND rd, rs1, rs2*/
    /*rd <- rs1 & rs2*/
    void AND(Mem& memory, Command command) {
        memory.registers[command.param1] = memory.registers[command.param2] & memory.registers[command.offset];

        if (memory.registers[command.param1] == 0) _cpu->SetZFlag();
        if (memory.registers[command.param1] & (1 << 31)) _cpu->SetSFlag();
        else _cpu->ResetSFlag();

        std::cout << "AND rd = " << std::bitset<32>(memory.registers[command.param1]) << std::endl;
    }

    /*OR rd, rs1, rs2*/
    /*rd <- rs1 | rs2*/
    void OR(Mem& memory, Command command) {
        memory.registers[command.param1] = memory.registers[command.param2] | memory.registers[command.offset];

        if (memory.registers[command.param1] == 0) _cpu->SetZFlag();
        if (memory.registers[command.param1] & (1 << 31)) _cpu->SetSFlag();
        else _cpu->ResetSFlag();

        std::cout << "OR rd = " << std::bitset<32>(memory.registers[command.param1]) << std::endl;
    }

    /*XOR rd, rs1, rs2*/
    /*rd <- rs1 ^ rs2*/
    void XOR(Mem& memory, Command command) {
        memory.registers[command.param1] = memory.registers[command.param2] ^ memory.registers[command.offset];

        if (memory.registers[command.param1] == 0) _cpu->SetZFlag();
        if (memory.registers[command.param1] & (1 << 31)) _cpu->SetSFlag();
        else _cpu->ResetSFlag();

        std::cout << "XOR rd = " << std::bitset<32>(memory.registers[command.param1]) << std::endl;
    }

    /*SLL rd, rs1, rs2*/
    /*rd <- rs1 << rs2*/
    void SLL(Mem& memory, Command command) {
        memory.registers[command.param1] = memory.registers[command.param2] << memory.registers[command.offset];

        auto test_result = static_cast<long long>(memory.registers[command.param2]) << memory.registers[command.offset];
        if (test_result > INT32_MAX || test_result < INT32_MIN) {
            _cpu->SetOFlag();
            _cpu->SetCFlag();
        }

        std::cout << "SLL rd = " << std::bitset<32>(memory.registers[command.param1]) << std::endl;
    }

    /*SRL rd, rs1, rs2*/
    /*rd <- rs1 >> rs2*/
    void SRL(Mem& memory, Command command) {
        memory.registers[command.param1] = memory.registers[command.param2] >> memory.registers[command.offset];

        auto test_result = static_cast<long long>(memory.registers[command.param2]) >> memory.registers[command.offset];
        if (test_result > INT32_MAX || test_result < INT32_MIN) {
            _cpu->SetOFlag();
            _cpu->SetCFlag();
        }

        std::cout << "SRL rd = " << std::bitset<32>(memory.registers[command.param1]) << std::endl;
    }

    /*SRA rd, rs1, rs2*/
    /*rd <- rs1 << rs2*/
    void SRA(Mem& memory, Command command) {
        bool temp = (bool((1 << 31) & memory.registers[command.param2]));
        Word shift = memory.registers[command.offset];

        auto test_result = static_cast<long long>(memory.registers[command.param2]);

        for (int i = 0; i < shift; ++i) {
            memory.registers[command.param1] = memory.registers[command.param2] << 1;
            test_result = memory.registers[command.param2] << 1;
            if (temp == 1) {
                memory.registers[command.param1] |= temp << 31;
                test_result |= temp << 31;
            } else {
                memory.registers[command.param1] &= ~(1 << 31);
                test_result &= ~(1 << 31);
            }
        }

        if (test_result > INT32_MAX || test_result < INT32_MIN) {
            _cpu->SetOFlag();
            _cpu->SetCFlag();
        }

        std::cout << "SRA rd = " << std::bitset<32>(memory.registers[command.param1]) << std::endl;
    }



    //load/store operations
    /*LW rd, imm13(rs1)*/
    /*rd <- mem[rs1 + imm13]*/
    void LW(Mem& memory, Command command) {
        memory.registers[command.param1] = memory.GetWordFromMem(command.param2 + command.offset);
        std::cout << "LW rd = " << std::bitset<32>(memory.registers[command.param1]) << std::endl;
        std::cout << "as int: " << memory.registers[command.param1] << '\n';
    }

    /*LH rd, imm13(rs1)*/
    /*rd <- mem[rs1 + imm13]*/
    void LH(Mem& memory, Command command) {
        memory.registers[command.param1] = memory.GetHalfwordFromMem(command.param2 + command.offset);
        std::cout << "LH rd = " << std::bitset<32>(memory.registers[command.param1]) << std::endl;
    }

    /*LB rd, imm13(rs1)*/
    /*rd <- mem[rs1 + imm13]*/
    void LB(Mem& memory, Command command) {
        memory.registers[command.param1] = memory.GetByteFromMem(command.param2 + command.offset);
        std::cout << "LH rd = " << std::bitset<32>(memory.registers[command.param1]) << std::endl;
    }

    /*SW rs1, imm13(rs2)*/
    /*rs1 -> mem[rs2 + imm13]*/
    void SW(Mem& memory, Command command) {
        memory.SetWordToMem(command.param2 + command.offset, memory.registers[command.param1]);
        std::cout << "SW rd = " << std::bitset<32>(memory.GetWordFromMem(command.param2 + command.offset)) << std::endl;
    }

    /*SH rs1, imm13(rs2)*/
    /*rs1 -> mem[rs2 + imm13]*/
    void SH(Mem& memory, Command command) {
        memory.SetHalfwordToMem(command.param2 + command.offset, memory.registers[command.param1] >> 16);
        std::cout << "SW rd = " << std::bitset<32>(memory.GetWordFromMem(command.param2 + command.offset)) << std::endl;
    }

    /*SB rs1, imm13(rs2)*/
    /*rs1 -> mem[rs2 + imm13]*/
    void SB(Mem& memory, Command command) {
        memory.SetByteToMem(command.param2 + command.offset, memory.registers[command.param1] >> 8);
        std::cout << "SW rd = " << std::bitset<32>(memory.GetWordFromMem(command.param2 + command.offset)) << std::endl;
    }

    /*IN rd, rs1*/
    /*Port[rs1] -> rd*/
    void IN(Mem& memory, Command command) {
        memory.registers[command.param1] = memory.port[command.param2];
    }

    /*OUT rd, rs1*/
    /*rs1 -> Port[rd]*/
    void OUT(Mem& memory, Command command) {
        memory.port[command.param1] = memory.registers[command.param2];
    }

    /*MOV rd, rs1*/
    /*rs1 -> rd*/
    void MOV(Mem& memory, Command command) {
        memory.registers[command.param1] = memory.registers[command.param2];
        std::cout << "MOV rd = " << std::bitset<32>(memory.registers[command.param1]) << std::endl;
    }

    /*SWP rd, rs1*/
    /*rs1 <-> rd*/
    void SWP(Mem& memory, Command command) {
        Word tmp = memory.registers[command.param1];
        memory.registers[command.param1] = memory.registers[command.param2];
        memory.registers[command.param2] = tmp;
        std::cout << "SWP rd = " << std::bitset<32>(memory.registers[command.param1]) << std::endl;
        std::cout << "SWP rs1 = " << std::bitset<32>(memory.registers[command.param2]) << std::endl;
    }



    //command flow control
    /*CLI*/
    /*Clear Interrupt flag*/
    void CLI() {
        _cpu->ResetIFlag();
    }

    /*STI*/
    /*Set Interrupt flag*/
    void STI() {
        _cpu->SetIFlag();
    }

    /*INT*/
    /*INT rd*/
    /*rd - interrupt table index*/
    /*PC -> PCI*/
    /*FLAGS -> FLAGSI*/
    void INT(Mem& memory, Command command) {
        _cpu->interruptCode = command.param1;
        _cpu->PCI = _cpu->PC;
        _cpu->FLAGSI = _cpu->FLAGS;
    }

    /*IRET*/
    /*Interrupt return*/
    /*PCI -> PC*/
    /*FLAGSI -> FLAGS*/
    void IRET(Mem& memory, Command command) {
        _cpu->PC = _cpu->PCI;
        _cpu->FLAGS = _cpu->FLAGSI;
    }

    /*CALL r1, imm13*/
    /*Subprogram call*/
    /*PC -> SYSREG*/
    /*PC = r1 + imm13*/
    void CALL(Mem& memory, Command command) {
        _cpu->SYSREG = _cpu->PC;
        _cpu->PC = memory.registers[command.param1] + command.offset;
    }

    /*RET*/
    /*PC = SYSREG*/
    void RET(Mem& memory, Command command) {
        _cpu->PC = _cpu->SYSREG;
    }

    /*JMP r1, imm13*/
    /*PC = r1 + imm13*/
    void JMP(Mem& memory, Command command) {
        _cpu->PC = memory.registers[command.param1] + command.offset;
    }

    /*JZ r1, imm13*/
    /*PC = r1 + imm13*/
    void JZ(Mem& memory, Command command) {
        if (_cpu->FLAGS & (1 << 7)) {
            _cpu->PC = memory.registers[command.param1] + command.offset;
        }
    }

    /*JNZ r1, imm13*/
    /*PC = r1 + imm13*/
    void JNZ(Mem& memory, Command command) {
        if (!(_cpu->FLAGS & (1 << 7))) {
            _cpu->PC = memory.registers[command.param1] + command.offset;
        }
    }

    /*JO r1, imm13*/
    /*PC = r1 + imm13*/
    void JO(Mem& memory, Command command) {
        if (_cpu->FLAGS & (1 << 4)) {
            _cpu->PC = memory.registers[command.param1] + command.offset;
        }
    }

    /*JNO r1, imm13*/
    /*PC = r1 + imm13*/
    void JNO(Mem& memory, Command command) {
        if (!(_cpu->FLAGS & (1 << 4))) {
            _cpu->PC = memory.registers[command.param1] + command.offset;
        }
    }

    /*JC r1, imm13*/
    /*PC = r1 + imm13*/
    void JC(Mem& memory, Command command) {
        if (_cpu->FLAGS & (1 << 6)) {
            _cpu->PC = memory.registers[command.param1] + command.offset;
        }
    }

    /*JNC r1, imm13*/
    /*PC = r1 + imm13*/
    void JNC(Mem& memory, Command command) {
        if (!(_cpu->FLAGS & (1 << 6))) {
            _cpu->PC = memory.registers[command.param1] + command.offset;
        }
    }

    /*JS r1, imm13*/
    /*PC = r1 + imm13*/
    void JS(Mem& memory, Command command) {
        if (_cpu->FLAGS & (1 << 5)) {
            _cpu->PC = memory.registers[command.param1] + command.offset;
        }
    }

    /*JNS r1, imm13*/
    /*PC = r1 + imm13*/
    void JNS(Mem& memory, Command command) {
        if (!(_cpu->FLAGS & (1 << 5))) {
            _cpu->PC = memory.registers[command.param1] + command.offset;
        }
    }



    //Special operations
    /*NOP*/
    /*Empty operation*/
    void NOP() {
        if (_cpu->FLAGS & (1 << 1)) _cpu->PC++;
        else _cpu->interruptCode = 3;
    }

    /*RFL rd*/
    /*FLAGS -> rd*/
    void RFL(Mem& memory, Command command) {
        if (_cpu->FLAGS & (1 << 1))
            memory.registers[command.param1] = _cpu->FLAGS;
        else _cpu->interruptCode = 3;
    }

    /*WFL r1*/
    /*r1 -> FLAGS*/
    void WFL(Mem& memory, Command command) {
        if (_cpu->FLAGS & (1 << 1))
            _cpu->FLAGS = memory.registers[command.param1] & 0xFF;
        else _cpu->interruptCode = 3;
    }

    /*RTLB rd*/
    /*TLB -> rd*/
    void RTLB(Mem& memory, Command command) {
        if (_cpu->FLAGS & (1 << 1))
            memory.registers[command.param1] = _cpu->TLB;
        else _cpu->interruptCode = 3;
    }

    /*WTLB r1*/
    /*r1 -> TLB*/
    void WTLB(Mem& memory, Command command) {
        if (_cpu->FLAGS & (1 << 1))
            _cpu->TLB = memory.registers[command.param1];
        else _cpu->interruptCode = 3;
    }

    /*RFE*/
    /*Enter superuser mode*/
    void RFE() {
        _cpu->SetUFlag();
    }

    /*SCALL*/
    /*Exit superuser mode*/
    void SCALL() {
        if (_cpu->FLAGS & (1 << 1))
            _cpu->ResetUFlag();
        else _cpu->interruptCode = 3;
    }
}
