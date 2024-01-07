

#include "InstructionTable.h"
#include <iomanip>

OperationDescription instructionTable[255];

std::map<Register, const char*> registerToString = {
	{ UNKNOWNREG, "unknown" },
	{ AL, "al"},
	{ AH, "ah"},
	{ AX, "ax"},
	{ BL, "bl"},
	{ BH, "bh"},
	{ BX, "bx"},
	{ CL, "cl"},
	{ CH, "ch"},
	{ CX, "cx"},
	{ DL, "dl"},
	{ DH, "dh"},
	{ DX, "dx"},
	{ DI, "di"},
	{ SI, "si"},
	{ BP, "bp"},
	{ SP, "sp"}
};

std::map<OperationType, const char*> operationToString = {
	{ UNKNOWNOPS, "unknown" },
	{ IMMEDIATEOPS, "immediateops" },
	{ MOV, "mov"},
	{ ADD, "add"},
	{ SUB, "sub"},
	{ CMP, "cmp"},
	{ JUMP, "jump"},
	{ JNZ, "jnz"},
	{ JE, "je"},
	{ JL, "jl"},
	{ JLE, "jle"},
	{ JB, "jb"},
	{ JBE, "jbe"},
	{ JP, "jp"},
	{ JO, "jo"},
	{ JS, "js"},
	{ JNE, "jne"},
	{ JNL, "jnl"},
	{ JG, "jg"},
	{ JNB, "jnb"},
	{ JA, "ja"},
	{ JNP, "jnp"},
	{ JNO, "jno"},
	{ JNS, "jns"},
	{ LOOP, "loop"},
	{ LOOPZ, "loopz"},
	{ LOOPNZ, "loopnz"},
	{ JCXZ, "jcxz"}
};

void initOperationTable()
{
	// MOVE
	instructionTable[136] = { MOV, true, false, false, UNKNOWNREG }; // move 100010DW
	instructionTable[137] = { MOV, true, false, false, UNKNOWNREG }; // move 100010DW
	instructionTable[138] = { MOV, true, false, false, UNKNOWNREG }; // move 100010DW
	instructionTable[139] = { MOV, true, false, false, UNKNOWNREG }; // move 100010DW
	instructionTable[176] = { MOV, false, true, false, UNKNOWNREG }; // immediateMoveOps 1011WREG
	instructionTable[177] = { MOV, false, true, false, UNKNOWNREG }; // immediateMoveOps 1011WREG
	instructionTable[178] = { MOV, false, true, false, UNKNOWNREG }; // immediateMoveOps 1011WREG
	instructionTable[179] = { MOV, false, true, false, UNKNOWNREG }; // immediateMoveOps 1011WREG
	instructionTable[180] = { MOV, false, true, false, UNKNOWNREG }; // immediateMoveOps 1011WREG
	instructionTable[181] = { MOV, false, true, false, UNKNOWNREG }; // immediateMoveOps 1011WREG
	instructionTable[182] = { MOV, false, true, false, UNKNOWNREG }; // immediateMoveOps 1011WREG
	instructionTable[183] = { MOV, false, true, false, UNKNOWNREG }; // immediateMoveOps 1011WREG
	instructionTable[184] = { MOV, false, true, false, UNKNOWNREG }; // immediateMoveOps 1011WREG
	instructionTable[185] = { MOV, false, true, false, UNKNOWNREG }; // immediateMoveOps 1011WREG
	instructionTable[186] = { MOV, false, true, false, UNKNOWNREG }; // immediateMoveOps 1011WREG
	instructionTable[187] = { MOV, false, true, false, UNKNOWNREG }; // immediateMoveOps 1011WREG
	instructionTable[188] = { MOV, false, true, false, UNKNOWNREG }; // immediateMoveOps 1011WREG
	instructionTable[189] = { MOV, false, true, false, UNKNOWNREG }; // immediateMoveOps 1011WREG
	instructionTable[190] = { MOV, false, true, false, UNKNOWNREG }; // immediateMoveOps 1011WREG
	instructionTable[191] = { MOV, false, true, false, UNKNOWNREG }; // immediateMoveOps 1011WREG
	// ADD
	instructionTable[0] = { ADD, true, false, false, UNKNOWNREG }; // add 000000DW
	instructionTable[1] = { ADD, true, false, false, UNKNOWNREG }; // add 000000DW
	instructionTable[2] = { ADD, true, false, false, UNKNOWNREG }; // add 000000DW
	instructionTable[3] = { ADD, true, false, false, UNKNOWNREG }; // add 000000DW
	instructionTable[128] = { IMMEDIATEOPS, true, true, false, UNKNOWNREG }; // immediateAddOps 100000SW
	instructionTable[129] = { IMMEDIATEOPS, true, true, false, UNKNOWNREG }; // immediateAddOps 100000SW
	instructionTable[130] = { IMMEDIATEOPS, true, true, false, UNKNOWNREG }; // immediateAddOps 100000SW
	instructionTable[131] = { IMMEDIATEOPS, true, true, false, UNKNOWNREG }; // immediateAddOps 100000SW
	instructionTable[4] = { ADD, false, true, false, AL }; // add with carry 0000010W
	instructionTable[5] = { ADD, false, true, false, AX }; // add with carry 0000010W
	//SUB
	instructionTable[40] = { SUB, true, false, false, UNKNOWNREG }; // sub 001010DW
	instructionTable[41] = { SUB, true, false, false, UNKNOWNREG }; // sub 001010DW
	instructionTable[42] = { SUB, true, false, false, UNKNOWNREG }; // sub 001010DW
	instructionTable[43] = { SUB, true, false, false, UNKNOWNREG }; // sub 001010DW
	instructionTable[128] = { IMMEDIATEOPS, true, true, false, UNKNOWNREG }; // immediateSubOps 100000SW
	instructionTable[129] = { IMMEDIATEOPS, true, true, false, UNKNOWNREG }; // immediateSubOps 100000SW
	instructionTable[130] = { IMMEDIATEOPS, true, true, false, UNKNOWNREG }; // immediateSubOps 100000SW
	instructionTable[131] = { IMMEDIATEOPS, true, true, false, UNKNOWNREG }; // immediateSubOps 100000SW
	instructionTable[44] = { SUB, false, true, false, AL }; // sub with carry 0010110W
	instructionTable[45] = { SUB, false, true, false, AX }; // sub with carry 0010110W
	//SUB
	instructionTable[56] = { CMP, true, false, false, UNKNOWNREG }; // cmp 001110DW
	instructionTable[57] = { CMP, true, false, false, UNKNOWNREG }; // cmp 001110DW
	instructionTable[58] = { CMP, true, false, false, UNKNOWNREG }; // cmp 001110DW
	instructionTable[59] = { CMP, true, false, false, UNKNOWNREG }; // cmp 001110DW
	instructionTable[128] = { IMMEDIATEOPS, true, true, false, UNKNOWNREG }; // immediateCmpOps 100000SW
	instructionTable[129] = { IMMEDIATEOPS, true, true, false, UNKNOWNREG }; // immediateCmpOps 100000SW
	instructionTable[130] = { IMMEDIATEOPS, true, true, false, UNKNOWNREG }; // immediateCmpOps 100000SW
	instructionTable[131] = { IMMEDIATEOPS, true, true, false, UNKNOWNREG }; // immediateCmpOps 100000SW
	instructionTable[60] = { CMP, false, true, false, AL }; // cmp with carry 0011110W
	instructionTable[61] = { CMP, false, true, false, AX }; // cmp with carry 0011110W

	// JE
	instructionTable[116] = { JE, false, false, true,      UNKNOWNREG };  // je     01110100
	instructionTable[117] = { JNE, false, false, true,     UNKNOWNREG };  // jne    01110101
	instructionTable[124] = { JL, false, false, true,      UNKNOWNREG };  // jl     01111100
	instructionTable[126] = { JLE, false, false, true,     UNKNOWNREG };  // jle    01111110
	instructionTable[114] = { JB, false, false, true,      UNKNOWNREG };  // jb     01110010
	instructionTable[118] = { JBE, false, false, true,     UNKNOWNREG };  // jbe    01110110
	instructionTable[122] = { JP, false, false, true,      UNKNOWNREG };  // jp     01111010
	instructionTable[112] = { JO, false, false, true,      UNKNOWNREG };  // jo     01110000
	instructionTable[120] = { JS, false, false, true,      UNKNOWNREG };  // js     01111000
	instructionTable[125] = { JNL, false, false, true,     UNKNOWNREG };  // jnl    01111101
	instructionTable[127] = { JG, false, false, true,      UNKNOWNREG };  // jg     01111111
	instructionTable[115] = { JNB, false, false, true,     UNKNOWNREG };  // jnb    01110011
	instructionTable[119] = { JA, false, false, true,      UNKNOWNREG };  // ja     01110111
	instructionTable[123] = { JNP, false, false, true,     UNKNOWNREG };  // jnp    01111011
	instructionTable[113] = { JNO, false, false, true,     UNKNOWNREG };  // jno    01110001
	instructionTable[121] = { JNS, false, false, true,     UNKNOWNREG };  // jns    01111001
	instructionTable[226] = { LOOP, false, false, true,    UNKNOWNREG };  // loop   11100010
	instructionTable[225] = { LOOPZ, false, false, true,   UNKNOWNREG };  // loopz  11100001
	instructionTable[224] = { LOOPNZ, false, false, true,  UNKNOWNREG };  // loopnz 11100000
	instructionTable[227] = { JCXZ, false, false, true,    UNKNOWNREG };  // jcxz   11100011
}

void printLocation(const Location& location, std::ofstream& output)
{
	// reg + offset
	if (location.offset.reg != Register::UNKNOWNREG || location.offset.number != 0)
	{
		output << "[";
		if (location.reg != Register::UNKNOWNREG) {
			output << registerToString[location.reg];
		}
		// offset register
		if (location.reg != Register::UNKNOWNREG && location.offset.reg != 0)
		{
			output << " + " << registerToString[location.offset.reg];
		}
		else if (location.offset.reg != UNKNOWNREG)
		{
			output << registerToString[location.offset.reg];
		}
		// offset number
		if ((location.reg != Register::UNKNOWNREG || location.offset.reg != UNKNOWNREG) && location.offset.number != 0)
		{
			output << " + ";
		}
		if (location.offset.number != 0)
		{
			char offsetStr[16] = {};
			sprintf(offsetStr, "%d", location.offset.number);
			output << offsetStr;
		}
		output << "]";
	}
	// only address
	else if (location.offset.number)
	{
		char offsetStr[16] = {};
		sprintf(offsetStr, "%d", location.offset.number);
		output << offsetStr;
	}
	// only reg
	else if (location.reg != Register::UNKNOWNREG) {
		output << registerToString[location.reg];
	}
}


uint8_t registerMemory[16] = {};
SimulationRegister ax = { AX, (void*)&registerMemory[0], 2 };
SimulationRegister al = { AL, (void*)&registerMemory[1], 1 };
SimulationRegister ah = { AH, (void*)&registerMemory[0], 1 };
SimulationRegister bx = { BX, (void*)&registerMemory[2], 2 };
SimulationRegister bl = { BL, (void*)&registerMemory[3], 1 };
SimulationRegister bh = { BH, (void*)&registerMemory[2], 1 };
SimulationRegister cx = { CX, (void*)&registerMemory[4], 2 };
SimulationRegister cl = { CL, (void*)&registerMemory[5], 1 };
SimulationRegister ch = { CH, (void*)&registerMemory[4], 1 };
SimulationRegister dx = { DX, (void*)&registerMemory[6], 2 };
SimulationRegister dl = { DL, (void*)&registerMemory[7], 1 };
SimulationRegister dh = { DH, (void*)&registerMemory[6], 1 };
SimulationRegister sp = { SP, (void*)&registerMemory[8], 2 };
SimulationRegister bp = { BP, (void*)&registerMemory[10], 2 };
SimulationRegister si = { SI, (void*)&registerMemory[12], 2 };
SimulationRegister di = { DI, (void*)&registerMemory[14], 2 };

std::map<Register, SimulationRegister*> registerToSimulationRegister = {
	{ AX, &ax }, { AL, &al }, { AH, &ah },
	{ BX, &bx }, { BL, &bl }, { BH, &bh },
	{ CX, &cx }, { CL, &cl }, { CH, &ch },
	{ DX, &dx }, { DL, &dl }, { DH, &dh },
	{ SP, &sp },
	{ BP, &bp },
	{ SI, &si },
	{ DI, &di },
};

void executeOperation(const Operation& currentOp)
{
	if (currentOp.op == MOV)
	{
		// to register
		if (currentOp.dst.offset.reg == UNKNOWNREG && currentOp.dst.offset.number == 0)
		{
			SimulationRegister* dstRegister = registerToSimulationRegister[currentOp.dst.reg];
			// load immadiate
			if (currentOp.src.reg == UNKNOWNREG || currentOp.src.offset.reg == UNKNOWNREG && currentOp.src.offset.number == 0)
			{
				if (dstRegister->size == 2) {
					dstRegister->mem = memcpy(dstRegister->mem, &currentOp.immediate, 2);
				} 
				else if (dstRegister->size == 1)
				{
					// copy low byte
					char* src = (char*)&currentOp.immediate;
					dstRegister->mem = memcpy(dstRegister->mem, (src + 1), 2);
				}
			}
		}
	}
}

void printSimulationTable(std::ofstream& output)
{
	output << "final register:" << std::endl;
	output << std::setw(2) << std::setfill('0');
	output << "      " << "ax: " << "0x" << std::setw(2) << std::setfill('0') << std::hex << *((int16_t*)ax.mem) << std::dec << " (" << *((int16_t*)ax.mem) << ")" << std::endl;
	output << "      " << "bx: " << "0x" << std::setw(2) << std::setfill('0') << std::hex << *((int16_t*)bx.mem) << std::dec << " (" << *((int16_t*)bx.mem) << ")" << std::endl;
	output << "      " << "cx: " << "0x" << std::setw(2) << std::setfill('0') << std::hex << *((int16_t*)cx.mem) << std::dec << " (" << *((int16_t*)cx.mem) << ")" << std::endl;
	output << "      " << "dx: " << "0x" << std::setw(2) << std::setfill('0') << std::hex << *((int16_t*)dx.mem) << std::dec << " (" << *((int16_t*)dx.mem) << ")" << std::endl;
	output << "      " << "sp: " << "0x" << std::setw(2) << std::setfill('0') << std::hex << *((int16_t*)sp.mem) << std::dec << " (" << *((int16_t*)sp.mem) << ")" << std::endl;
	output << "      " << "bp: " << "0x" << std::setw(2) << std::setfill('0') << std::hex << *((int16_t*)bp.mem) << std::dec << " (" << *((int16_t*)bp.mem) << ")" << std::endl;
	output << "      " << "si: " << "0x" << std::setw(2) << std::setfill('0') << std::hex << *((int16_t*)si.mem) << std::dec << " (" << *((int16_t*)si.mem) << ")" << std::endl;
	output << "      " << "di: " << "0x" << std::setw(2) << std::setfill('0') << std::hex << *((int16_t*)di.mem) << std::dec << " (" << *((int16_t*)di.mem) << ")" << std::endl;
}

void printOperation(const Operation& operation, std::ofstream& output)
{
	output << operationToString[operation.op] << " ";
	if (operation.hasByteKeyword) {
		output << "byte ";
	}
	else if (operation.hasWordKeyword) {
		output << "word ";
	}
	printLocation(operation.dst, output);
	output << ", ";
	if (operation.immediate != 0) {
		output << operation.immediate;
	}
	else {
		printLocation(operation.src, output);
	}
}