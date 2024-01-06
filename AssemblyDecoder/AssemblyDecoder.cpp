
#include "InstructionTable.h"

#include <iostream>
#include <fstream>
#include <vector>

#define _CRT_SECURE_NO_WARNINGS

/*
 *  /g/Tools/Nasm/nasm.exe simple.asm -> simple.asm
 *  /g/Tools/Nasm/nasm.exe result.asm -> result
 *  diff simple result
*/

std::vector<Register> registerMapping8bits = {
	AL, CL, DL, BL, AH, CH, DH, BH
};

std::vector<Register> registerMapping16bits = {
	AX, CX, DX, BX, SP, BP, SI, DI
};

// "bx + si", "bx + di", "bp + si", "bp + di", "si", "di", "bp", "bx"
std::vector<Location> registerToMemory = {
	{ BX, { SI, 0} },
	{ BX, { DI, 0} },
	{ BP, { SI, 0} },
	{ BP, { DI, 0} },
	{ UNKNOWNREG, { SI, 0} },
	{ UNKNOWNREG, { DI, 0} },
	{ UNKNOWNREG, { BP, 0} },
	{ UNKNOWNREG, { BX, 0} },
};

void printBinary(unsigned char byte)
{
	char buffer[9] = {};
	for (uint32_t i = 0; i < 8; i++)
	{
		if (byte & 1) {
			buffer[7 - i] = '1';
		} else {
			buffer[7 - i] = '0';
		}
		byte = byte >> 1;
	}

	printf("%s\n", buffer);
}


//cmp word [4834], 29

void decodeInstruction(const std::vector<unsigned char>& data, size_t& index, Operation& result)
{
	unsigned char opCode = data[index];
	const OperationDescription& opDesc = instructionTable[opCode];
	if (opDesc.op != UNKNOWNOPS)
	{
		result.op = opDesc.op;
		// 000000DW
		bool isRegDestination, is16bitsOps;
		is16bitsOps = opCode & 1;
		isRegDestination = opCode & 2;

		// mov, add, sub, cmp
		// modregrm or modcodrm
		if (opDesc.hasTwoArguments && opDesc.op == IMMEDIATEOPS)
		{
			OperationType regToOperation[8];
			regToOperation[0] = ADD; // 000
			regToOperation[5] = SUB; // 101
			regToOperation[7] = CMP; // 111

			bool isSignedOperation = (opCode & 2);
			unsigned char argOps = data[index + 1];
			unsigned char rm = (argOps & 7);
			unsigned char reg = (argOps >> 3) & 7;
			unsigned char mod = (argOps >> 6) & 7;
			bool registerMode = mod == 3;
			index++;
			result.op = regToOperation[reg];
			if (registerMode) 
			{
				result.dst.reg = is16bitsOps ? registerMapping16bits[rm] : registerMapping8bits[rm];
			}
			else
			{
				bool isDirectMemory = (mod == 0) && rm == 6;
				result.hasWordKeyword = is16bitsOps;
				if (result.hasWordKeyword)
				{
					std::cout << "word";
				}
				result.hasByteKeyword = !is16bitsOps;
				if (!isDirectMemory) {
					result.dst = registerToMemory[rm];
				}
				// offset value
				int16_t offset = 0;
				if (isDirectMemory || mod == 2) {
					offset |= data[index + 2];
					offset = offset << 8;
					offset |= data[index + 1];
					index = index + 2;
				}
				else if (mod == 1) {
					int8_t shortOffset = data[index + 1];
					offset = shortOffset;
					index = index + 1;
				}
				result.dst.offset.number = isSignedOperation ? offset : static_cast<uint16_t>(offset);
			}

			if (is16bitsOps && !isSignedOperation) {
				result.immediate |= data[index + 2];
				result.immediate = result.immediate << 8;
				result.immediate |= data[index + 1];
				index = index + 2;
			}
			//else if (mod == 1) {
			else {
				int8_t shortOffset = data[index + 1];
				result.immediate = shortOffset;
				index = index + 1;
			}
		}
		else if (opDesc.hasTwoArguments) 
		{
			unsigned char argOps = data[index + 1];
			unsigned char rm = (argOps & 7);
			unsigned char reg = (argOps >> 3) & 7;
			unsigned char mod = (argOps >> 6) & 7;
			index++;

			bool registerMode = mod == 3;
			if (registerMode) 
			{
				if (is16bitsOps) {
					result.src.reg = isRegDestination ? registerMapping16bits[rm] : registerMapping16bits[reg];
					result.dst.reg = isRegDestination ? registerMapping16bits[reg] : registerMapping16bits[rm];
				}
				else {
					result.src.reg = isRegDestination ? registerMapping8bits[rm] : registerMapping8bits[reg];
					result.dst.reg = isRegDestination ? registerMapping8bits[reg] : registerMapping8bits[rm];
				}
			}
			else 
			{
				Location memoryLocation = registerToMemory[rm];
				bool isDirectMemory = (mod == 0) && rm == 6;
				uint16_t value = 0;
				if (isDirectMemory || mod == 2) {
					value |= data[index + 2];
					value = value << 8;
					value |= data[index + 1];
					index = index + 2;
				}
				else if (mod == 1) {
					int8_t shortOffset = data[index + 1];
					value = shortOffset;
					index = index + 1;
				}
				memoryLocation.offset.number = value;

				if (isRegDestination) {
					result.dst.reg = is16bitsOps ? registerMapping16bits[reg] : registerMapping8bits[reg];
					result.src = memoryLocation;
				}
				// store
				else {
					result.dst = memoryLocation;
					result.src.reg = is16bitsOps ? registerMapping16bits[reg] : registerMapping8bits[reg];
				}
			}
		}
		// immediate mov to register
		else if (opDesc.op == MOV && opDesc.hasData)
		{
			bool is16bitsOps;
			is16bitsOps = opCode & 8; // 4th bits
			unsigned char reg = opCode & 7;
			result.dst.reg = is16bitsOps ? registerMapping16bits[reg] : registerMapping8bits[reg];
			int16_t value = 0;
			if (is16bitsOps) {
				value |= data[index + 2];
				value = value << 8;
				value |= data[index + 1];
				index = index + 2;
			}
			else {
				int8_t shortOffset = data[index + 1];
				value = shortOffset;
				index = index + 1;
			}
			result.immediate = value;
		}
		// immediate add, sub, cmp
		else if (opDesc.hasData)
		{
			bool is16bitsOps;
			is16bitsOps = opCode & 1;
			result.dst.reg = opDesc.defaultRegister;
			int16_t value = 0;
			// fetch hi byte
			if (is16bitsOps) {
				value |= data[index + 2];
				value = value << 8;
				value |= data[index + 1];
				index = index + 2;
			}
			else {
				int8_t shortOffset = data[index + 1];
				value = shortOffset;
				index = index + 1;
			}
			result.immediate = value;
		}
		// je, jnz, jl, ...
		else if (opDesc.useIncrement)
		{
			char offsetStr[16] = {};
			int8_t value = 0;
			value = data[index+1];
			result.immediate = value;
			index++;
		}
	}
}

void parseBinary(const std::vector<unsigned char>& data)
{
	std::ofstream output;
	output.open("G:\\Projects\\TutoAssembly\\AssemblyDecoder\\ressources\\result.asm", std::ios::binary);
	output << "bits 16" << std::endl;

	std::vector<Operation> instructions(data.size());
	for (size_t byteIndex = 0; byteIndex < data.size(); )
	{
		Operation currentOp = {};
		decodeInstruction(data, byteIndex, currentOp);
		printOperation(currentOp, output);
		output << std::endl;
		byteIndex++;
	}
}

int main()
{
	std::ifstream input;
	input.open("G:\\Projects\\TutoAssembly\\AssemblyDecoder\\ressources\\ex3", std::ios::binary);

	input.seekg(0, std::ios::end);
	std::streampos fileSize = input.tellg();
	input.seekg(0, std::ios::beg);

	std::vector<unsigned char> fileData(fileSize);
	input.read((char*)&fileData[0], fileSize);

	std::cout << "reading file:" << std::endl;
	for (size_t i = 0; i < fileSize; i++)
	{
		printBinary(fileData[i]);
	}

	initOperationTable();
	parseBinary(fileData);
}