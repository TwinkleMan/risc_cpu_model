#include "cpu.h"

#include <bitset>

Word* CPU::GetPcPointer()
{ return &PC; }

Word* CPU::GetPciPointer()
{ return &PCI; }

Byte* CPU::GetFlagsPointer()
{ return &FLAGS; }

Byte* CPU::GetFlagsiPointer()
{ return &FLAGSI; }

Word* CPU::GetSysregPointer()
{ return &SYSREG; }

void CPU::Reset(Mem& memory)
{
	PC = 0x0000;
	FLAGS = 0b00000000;
	interruptCode = -1;
	memory.Initialise();
	_memory = &memory;
}

Byte CPU::FetchByte(u32& cycles, Mem& memory)
{
	Byte data = memory[PC];
	PC++;
	cycles--;
	return data;
}

Word CPU::FetchWord(u32& cycles, Mem& memory)
{
	Word data;
	data = memory[PC];
	PC++;
	cycles--;
	return data;
}

Command CPU::TranslateCommand(u32& cycles, Word data)
{
	Command result{};
	result.Clear();
	result.currentPC = &PC;

	std::cout << "data = " << std::bitset<32>(data) << std::endl;
	//7 bits for opcode
	for (u32 i = 0; i < 7; ++i) {
		result.opcode |= ((data >> (31 - i)) & 1) << (6 - i);
	}
	std::cout << "result.opcode = " << std::bitset<7>(result.opcode) << std::endl;

	//6 bits for param1 (R0)
	for (u32 i = 0; i < 6; ++i) {
		result.param1 |= ((data >> (24 - i)) & 1) << (5 - i);
	}
	std::cout << "result.param1 = " << std::bitset<6>(result.param1) << std::endl;
	//6 bits for param2 (R1)
	for (u32 i = 0; i < 6; ++i) {
		result.param2 |= ((data >> (18 - i)) & 1) << (5 - i);
	}
	std::cout << "result.param2 = " << std::bitset<6>(result.param2) << std::endl;

	//13 bits for constant\offset
	for (u32 i = 0; i < 13; ++i) {
		result.offset |= ((data >> (12 - i)) & 1) << (12 - i);
	}
	std::cout << "result.offset = " << std::bitset<13>(result.offset) << std::endl;

	cycles--;

	return result;
}

void CPU::RunCommand(Command& command, Mem& memory)
{
	command.Execute(memory);
}

std::string CPU::HandleInterruptions()
{
	std::string message;

	if ((interruptCode >= 0 && interruptCode <= 1)) {
		//interrupt (I flag check)
		if (FLAGS & (1 << 3)) {
			auto temp1 = _memory->interruptTable[interruptCode];
			message = (_memory->*temp1)();

			PC = PCI;
			FLAGS = FLAGSI;

			interruptCode = -1;
		} else {
			std::cout << "No interrupt allowed!" << std::endl;
		}
	} else if (interruptCode > -1 && interruptCode <= 5) {
		//exception (runs always, no I flag check)
		if (interruptCode == 4) stepMode = 1;
		auto temp1 = _memory->interruptTable[interruptCode];
		message = (_memory->*temp1)();

		//PC = PCI;
		//FLAGS = FLAGSI;

		interruptCode = -1;
	} else {
		std::cout << "No such interrupt!" << std::endl;
	}

	if (stepMode)
	{
		auto temp1 = _memory->interruptTable[4];
		message = (_memory->*temp1)();
	}

	return message;
}

void CPU::Execute(u32 cycles, Mem& memory)
{
	while (cycles > 0) {
		Word instruction = FetchWord(cycles, memory);
		Command command = TranslateCommand(cycles, instruction);
		//if (Command::_cpu == nullptr) Command::SetCPU(this);
		RunCommand(command,memory);
		HandleInterruptions();
	}
}

std::string CPU::Execute(Command& command, Mem& memory)
{
	//if (Command::_cpu == nullptr) Command::SetCPU(this);
	RunCommand(command, memory);
	return HandleInterruptions();
}

void CPU::SetZFlag()
{ FLAGS |= 1 << 7; }

void CPU::SetCFlag()
{ FLAGS |= 1 << 6; }

void CPU::SetSFlag()
{ FLAGS |= 1 << 5; }

void CPU::SetOFlag()
{ FLAGS |= 1 << 4; }

void CPU::SetIFlag()
{ FLAGS |= 1 << 3; }

void CPU::SetTFlag()
{ FLAGS |= 1 << 2; }

void CPU::SetUFlag()
{ FLAGS |= 1 << 1; }

void CPU::ResetZFlag()
{ FLAGS &= ~(1 << 7); }

void CPU::ResetCFlag()
{ FLAGS &= ~(1 << 6); }

void CPU::ResetSFlag()
{ FLAGS &= ~(1 << 5); }

void CPU::ResetOFlag()
{ FLAGS &= ~(1 << 4); }

void CPU::ResetIFlag()
{ FLAGS &= ~(1 << 3); }

void CPU::ResetTFlag()
{ FLAGS &= ~(1 << 2); }

void CPU::ResetUFlag()
{ FLAGS &= ~(1 << 1); }

Word CPU::GetPC()
{ return PC; }
