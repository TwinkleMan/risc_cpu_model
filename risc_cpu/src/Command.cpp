#include <iostream>

#include "Command.h"
#include "CommandList.h"

void Command::SetCpu(void* p_cpu)
{
    _cpu = p_cpu;
    auto temp = static_cast<CPU*>(_cpu);
    Commands::SetCpu(static_cast<CPU*>(p_cpu));
}

void Command::Execute(Mem &memory) {
    switch (opcode) {
        case ADD:
        {
            std::cout << "ADD    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2) << "    " << std::bitset<13>(offset) << std::endl;
            Commands::ADD(memory, *this);
            break;
        }
        case ADDU:
        {
            std::cout << "ADDU    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2) << "    " << std::bitset<13>(offset) << std::endl;
            Commands::ADDU(memory, *this);
            break;
        }
        case SUB: {
            std::cout << "SUB    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2) << "    " << std::bitset<13>(offset) << std::endl;
            Commands::SUB(memory, *this);
            break;
        }
        case SUBU: {
            std::cout << "SUBU    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2) << "    " << std::bitset<13>(offset) << std::endl;
            Commands::SUBU(memory, *this);
            break;
        }
        case ADDI: {
            //add immediate
            std::cout << "ADDI    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2) << "    " << std::bitset<13>(offset) << std::endl;
            Commands::ADDI(memory, *this);
            break;
        }
        case SLT: {
            std::cout << "SLT    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2) << "    " << std::bitset<13>(offset) << std::endl;
            Commands::SLT(memory, *this);
            break;
        }
        case SLTI: {
            std::cout << "SLTI    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2) << "    " << std::bitset<13>(offset) << std::endl;
            Commands::SLTI(memory, *this);
            break;
        }
        case SLTU: {
            std::cout << "SLTU    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2) << "    " << std::bitset<13>(offset) << std::endl;
            Commands::SLTU(memory, *this);
            break;
        }
        case SLTIU: {
            std::cout << "SLTIU    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2) << "    " << std::bitset<13>(offset) << std::endl;
            Commands::SLTIU(memory, *this);
            break;
        }
        case LUI: {
            std::cout << "LUI    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2) << "    " << std::bitset<13>(offset) << std::endl;
            Commands::LUI(memory, *this);
            break;
        }
        case AUIP: {
            std::cout << "AUIP    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2) << "    " << std::bitset<13>(offset) << std::endl;
            Commands::AUIP(memory, *this);
            break;
        }
        case MUL: {
            std::cout << "MUL    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2) << "    " << std::bitset<13>(offset) << std::endl;
            Commands::MUL(memory, *this);
            break;
        }
        case MULU: {
            std::cout << "MULU    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2) << "    " << std::bitset<13>(offset) << std::endl;
            Commands::MULU(memory, *this);
            break;
        }
        case DIV: {
            std::cout << "DIV    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2) << "    " << std::bitset<13>(offset) << std::endl;
            Commands::DIV(memory, *this);
            break;
        }
        case DIVU: {
            std::cout << "DIVU    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2) << "    " << std::bitset<13>(offset) << std::endl;
            Commands::DIVU(memory, *this);
            break;
        }
        //floating point arithmetics
        case ADDF: {
            std::cout << "ADDF    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2) << "    " << std::bitset<13>(offset) << std::endl;
            Commands::ADDF(memory, *this);
            break;
        }
        case SUBF: {
            std::cout << "SUBF    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2) << "    " << std::bitset<13>(offset) << std::endl;
            Commands::SUBF(memory, *this);
            break;
        }
        case MULF: {
            std::cout << "MULF    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2) << "    " << std::bitset<13>(offset) << std::endl;
            Commands::MULF(memory, *this);
            break;
        }
        case DIVF: {
            std::cout << "DIVF    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2) << "    " << std::bitset<13>(offset) << std::endl;
            Commands::DIVF(memory, *this);
            break;
        }
        case TFIX: {
            std::cout << "TFIX    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2)<< std::endl;
            Commands::TFIX(memory, *this);
            break;
        }
        case TFLT: {
            std::cout << "TFLT    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2)<< std::endl;
            Commands::TFLT(memory, *this);
            break;
        }
        //Logical operations
        case AND: {
            std::cout << "SVB    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2) << "    " << std::bitset<13>(offset) << std::endl;
            Commands::AND(memory, *this);
            break;
        }
        case OR: {
            std::cout << "OR    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2) << "    " << std::bitset<13>(offset) << std::endl;
            Commands::OR(memory, *this);
            break;
        }
        case XOR: {
            std::cout << "XOR    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2) << "    " << std::bitset<13>(offset) << std::endl;
            Commands::XOR(memory, *this);
            break;
        }
        case SLL: {
            std::cout << "SLL    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2) << "    " << std::bitset<13>(offset) << std::endl;
            Commands::SLL(memory, *this);
            break;
        }
        case SRL: {
            std::cout << "SRL    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2) << "    " << std::bitset<13>(offset) << std::endl;
            Commands::SRL(memory, *this);
            break;
        }
        case SRA: {
            std::cout << "SRA    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2) << "    " << std::bitset<13>(offset) << std::endl;
            Commands::SRA(memory, *this);
            break;
        }
        //Load/store operations
        case LW: {
            std::cout << "LW    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2) << "    " << std::bitset<13>(offset) << std::endl;
            Commands::LW(memory, *this);
            break;
        }
        case LH: {
            std::cout << "LH    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2) << "    " << std::bitset<13>(offset) << std::endl;
            Commands::LH(memory, *this);
            break;
        }
        case LB: {
            std::cout << "LB    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2) << "    " << std::bitset<13>(offset) << std::endl;
            Commands::LB(memory, *this);
            break;
        }
        case SW: {
            std::cout << "SW    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2) << "    " << std::bitset<13>(offset) << std::endl;
            Commands::SW(memory, *this);
            break;
        }
        case SH: {
            std::cout << "SH    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2) << "    " << std::bitset<13>(offset) << std::endl;
            Commands::SH(memory, *this);
            break;
        }
        case SB: {
            std::cout << "SB    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2) << "    " << std::bitset<13>(offset) << std::endl;
            Commands::SB(memory, *this);
            break;
        }
        case IN: {
            std::cout << "IN    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2) << "    " << std::bitset<13>(offset) << std::endl;
            Commands::IN(memory, *this);
            break;
        }
        case OUT: {
            std::cout << "OUT    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2) << "    " << std::bitset<13>(offset) << std::endl;
            Commands::OUT(memory, *this);
            break;
        }
        case MOV: {
            std::cout << "MOV    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2) << std::endl;
            Commands::MOV(memory, *this);
            break;
        }
        case SWP: {
            std::cout << "SWP    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2) << std::endl;
            Commands::SWP(memory, *this);
            break;
        }
        //command flow control
        case CLI: {
            std::cout << "CLI" << std::endl;
            Commands::CLI();
            break;
        }
        case STI: {
            std::cout << "STI" << std::endl;
            Commands::STI();
            break;
        }
        case INT: {
            std::cout << "INT:    " << std::bitset<6>(param1) << std::endl;
            Commands::INT(memory, *this);
            break;
        }
        case IRET: {
            std::cout << "IRET" << std::endl;
            Commands::IRET(memory, *this);
            break;
        }
        case CALL: {
            std::cout << "CALL    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2) << "    " << std::bitset<13>(offset) << std::endl;
            Commands::CALL(memory, *this);
            break;
        }
        case RET: {
            std::cout << "RET    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2) << "    " << std::bitset<13>(offset) << std::endl;
            Commands::RET(memory, *this);
            break;
        }
        case JMP: {
            std::cout << "JMP    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2) << "    " << std::bitset<13>(offset) << std::endl;
            Commands::JMP(memory, *this);
            break;
        }
        case JZ: {
            std::cout << "JZ    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2) << "    " << std::bitset<13>(offset) << std::endl;
            Commands::JZ(memory, *this);
            break;
        }
        case JNZ: {
            std::cout << "JNZ    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2) << "    " << std::bitset<13>(offset) << std::endl;
            Commands::JNZ(memory, *this);
            break;
        }
        case JO: {
            std::cout << "JO    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2) << "    " << std::bitset<13>(offset) << std::endl;
            Commands::JO(memory, *this);
            break;
        }
        case JNO: {
            std::cout << "JNO    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2) << "    " << std::bitset<13>(offset) << std::endl;
            Commands::JNO(memory, *this);
            break;
        }
        case JC: {
            std::cout << "JC    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2) << "    " << std::bitset<13>(offset) << std::endl;
            Commands::JC(memory, *this);
            break;
        }
        case JNC: {
            std::cout << "JNC    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2) << "    " << std::bitset<13>(offset) << std::endl;
            Commands::JNC(memory, *this);
            break;
        }
        case JS: {
            std::cout << "JS    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2) << "    " << std::bitset<13>(offset) << std::endl;
            Commands::JS(memory, *this);
            break;
        }
        case JNS: {
            std::cout << "JNS    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2) << "    " << std::bitset<13>(offset) << std::endl;
            Commands::JNS(memory, *this);
            break;
        }
        //special operations
        case NOP: {
            std::cout << "NOP" << std::endl;
            Commands::NOP();
            break;
        }
        case RFL: {
            std::cout << "RFL    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2) << "    " << std::bitset<13>(offset) << std::endl;
            Commands::RFL(memory, *this);
            break;
        }
        case WFL: {
            std::cout << "WFL    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2) << "    " << std::bitset<13>(offset) << std::endl;
            Commands::WFL(memory, *this);
            break;
        }
        case RTLB: {
            std::cout << "RTLB    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2) << "    " << std::bitset<13>(offset) << std::endl;
            Commands::RTLB(memory, *this);
            break;
        }
        case WTLB: {
            std::cout << "WTLB    " << std::bitset<6>(param1) << "    " << std::bitset<6>(param2) << "    " << std::bitset<13>(offset) << std::endl;
            Commands::WTLB(memory, *this);
            break;
        }
        case RFE: {
            std::cout << "RFE" << std::endl;
            Commands::RFE();
            break;
        }
        case SCALL: {
            std::cout << "SCALL" << std::endl;
            Commands::SCALL();
            break;
        }



        case TEST: {
            Word tempB = -2147483648;
            //tempB = tempB | 255;
            Commands::convertToSigned(tempB);
        }

    }
};

void Command::Clear() {
    opcode = 0;
    param1 = param2 = 0;
    offset = 0;
    currentPC = nullptr;
}

Command BuildCommand(const char *data) {
    Command result{};
    result.Clear();

    std::string tmp(data);

    std::string operation, r0, r1, offset;

    int spacePos = tmp.find(' ');
    if (spacePos != -1) {
        operation = tmp.substr(0, spacePos);
    }
    tmp = tmp.substr(spacePos + 1, tmp.length() - spacePos);
    spacePos = tmp.find(' ');
    if (spacePos != -1) {
        r0 = tmp.substr(0, spacePos);
    }
    tmp = tmp.substr(spacePos + 1, tmp.length() - spacePos);
    spacePos = tmp.find(' ');
    if (spacePos != -1) {
        r1 = tmp.substr(0, spacePos);
    }
    tmp = tmp.substr(spacePos + 1, tmp.length() - spacePos);
    offset = tmp.substr(0, tmp.length());

    if (!operation.empty()) result.opcode = GetInfoFromText(operation);
    result.param1 = atoi(r0.c_str());
    result.param2 = atoi(r1.c_str());
    result.offset = atoi(offset.c_str());
    if (result.opcode == TFLT || result.opcode == TFIX || result.opcode == IN || result.opcode == OUT ||
            result.opcode == MOV || result.opcode == SWP) {
        result.param2 = result.offset;
        result.offset = 0;
    } else if ((result.opcode == LW) || (result.opcode == LH) || (result.opcode == LB) ||
            (result.opcode == SW) || (result.opcode == SH) || (result.opcode == SB)) {
        auto temp1 = offset.find('(');
        auto temp2 = offset.find(')');
        result.param2 = atoi(offset.substr(temp1 + 1, temp2 - temp1 - 1).c_str());
    } else if (result.opcode == INT || result.opcode == RFL || result.opcode == WFL || result.opcode == RTLB ||
            result.opcode == WTLB) {
        result.param1 = result.offset;
        result.offset = 0;
    } else if (operation.empty()) {
        result.opcode = GetInfoFromText(tmp);
    }


    return result;
}

Byte GetInfoFromText(std::string& operation) {
    Byte result;

    if (strcmp(operation.c_str(),"ADD") == 0) {
        result = ADD;
    }
    else if (strcmp(operation.c_str(),"ADDU") == 0) {
        result = ADDU;
    }
    else if (strcmp(operation.c_str(),"SUB") == 0) {
        result = SUB;
    }
    else if (strcmp(operation.c_str(),"SUBU") == 0) {
        result = SUBU;
    }
    else if (strcmp(operation.c_str(),"ADDI") == 0) {
        result = ADDI;
    }
    else if (strcmp(operation.c_str(),"SLT") == 0) {
        result = SLT;
    }
    else if (strcmp(operation.c_str(),"SLTI") == 0) {
        result = SLTI;
    }
    else if (strcmp(operation.c_str(),"SLTU") == 0) {
        result = SLTU;
    }
    else if (strcmp(operation.c_str(),"SLTIU") == 0) {
        result = SLTIU;
    }
    else if (strcmp(operation.c_str(),"LUI") == 0) {
        result = LUI;
    }
    else if (strcmp(operation.c_str(),"AUIP") == 0) {
        result = AUIP;
    }
    else if (strcmp(operation.c_str(),"MUL") == 0) {
        result = MUL;
    }
    else if (strcmp(operation.c_str(),"MULU") == 0) {
        result = MULU;
    }
    else if (strcmp(operation.c_str(),"DIV") == 0) {
        result = DIV;
    }
    else if (strcmp(operation.c_str(),"DIVU") == 0) {
        result = DIVU;
    }

    else if (strcmp(operation.c_str(),"ADDF") == 0) {
        result = ADDF;
    }
    else if (strcmp(operation.c_str(),"SUBF") == 0) {
        result = SUBF;
    }
    else if (strcmp(operation.c_str(),"MULF") == 0) {
        result = MULF;
    }
    else if (strcmp(operation.c_str(),"DIVF") == 0) {
        result = DIVF;
    }
    else if (strcmp(operation.c_str(),"TFIX") == 0) {
        result = TFIX;
    }
    else if (strcmp(operation.c_str(),"TFLT") == 0) {
        result = TFLT;
    }

    else if (strcmp(operation.c_str(),"AND") == 0) {
        result = AND;
    }
    else if (strcmp(operation.c_str(),"OR") == 0) {
        result = OR;
    }
    else if (strcmp(operation.c_str(),"XOR") == 0) {
        result = XOR;
    }
    else if (strcmp(operation.c_str(),"SLL") == 0) {
        result = SLL;
    }
    else if (strcmp(operation.c_str(),"SRL") == 0) {
        result = SRL;
    }
    else if (strcmp(operation.c_str(),"SRA") == 0) {
        result = SRA;
    }

    else if (strcmp(operation.c_str(),"LW") == 0) {
        result = LW;
    }
    else if (strcmp(operation.c_str(),"LH") == 0) {
        result = LH;
    }
    else if (strcmp(operation.c_str(),"LB") == 0) {
        result = LB;
    }
    else if (strcmp(operation.c_str(),"SW") == 0) {
        result = SW;
    }
    else if (strcmp(operation.c_str(),"SH") == 0) {
        result = SH;
    }
    else if (strcmp(operation.c_str(),"SB") == 0) {
        result = SB;
    }
    else if (strcmp(operation.c_str(),"MOV") == 0) {
        result = MOV;
    }
    else if (strcmp(operation.c_str(),"SWP") == 0) {
        result = SWP;
    }
    else if (strcmp(operation.c_str(),"IN") == 0) {
        result = IN;
    }
    else if (strcmp(operation.c_str(),"OUT") == 0) {
        result = OUT;
    }

    else if (strcmp(operation.c_str(),"CLI") == 0) {
        result = CLI;
    }
    else if (strcmp(operation.c_str(),"STI") == 0) {
        result = STI;
    }
    else if (strcmp(operation.c_str(),"INT") == 0) {
        result = INT;
    }
    else if (strcmp(operation.c_str(),"IRET") == 0) {
        result = IRET;
    }
    else if (strcmp(operation.c_str(),"CALL") == 0) {
        result = CALL;
    }
    else if (strcmp(operation.c_str(),"RET") == 0) {
        result = RET;
    }
    else if (strcmp(operation.c_str(),"JMP") == 0) {
        result = JMP;
    }
    else if (strcmp(operation.c_str(),"JZ") == 0) {
        result = JZ;
    }
    else if (strcmp(operation.c_str(),"JNZ") == 0) {
        result = JNZ;
    }
    else if (strcmp(operation.c_str(),"JO") == 0) {
        result = JO;
    }
    else if (strcmp(operation.c_str(),"JNO") == 0) {
        result = JNO;
    }
    else if (strcmp(operation.c_str(),"JC") == 0) {
        result = JC;
    }
    else if (strcmp(operation.c_str(),"JNC") == 0) {
        result = JNC;
    }
    else if (strcmp(operation.c_str(),"JS") == 0) {
        result = JS;
    }
    else if (strcmp(operation.c_str(),"JNS") == 0) {
        result = JNS;
    }

    else if (strcmp(operation.c_str(),"NOP") == 0) {
        result = NOP;
    }
    else if (strcmp(operation.c_str(),"RFL") == 0) {
        result = RFL;
    }
    else if (strcmp(operation.c_str(),"WFL") == 0) {
        result = WFL;
    }
    else if (strcmp(operation.c_str(),"RTLB") == 0) {
        result = RTLB;
    }
    else if (strcmp(operation.c_str(),"WTLB") == 0) {
        result = WTLB;
    }
    else if (strcmp(operation.c_str(),"RFE") == 0) {
        result = RFE;
    }
    else if (strcmp(operation.c_str(),"SCALL") == 0) {
        result = SCALL;
    }

    return result;
}
