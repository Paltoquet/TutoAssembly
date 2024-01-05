// AssemblyDecoder.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include "InstructionTable.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#define _CRT_SECURE_NO_WARNINGS

/*
 *  /g/Tools/Nasm/nasm.exe simple.asm -> simple.asm
 *  /g/Tools/Nasm/nasm.exe result.asm -> result
 *  diff simple result
*/

std::vector<const char*> registerMapping8bits = {
	"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"
};

std::vector<const char*> registerMapping16bits = {
	"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"
};

std::vector<const char*> registerToMemory = {
	"bx + si", "bx + di", "bp + si", "bp + di", "si", "di", "bp", "bx"
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

void executeOperation(const Operation& operation, std::ofstream& output, bool simulate)
{
	printOperation(operation, output);
	output << std::endl;
}



void decodeMove(const std::vector<unsigned char>& data, size_t& index, std::ofstream& output)
{
	const char* src = nullptr;
	const char* dst = nullptr;

	unsigned char movOps = data[index];
	unsigned char argOps = data[index + 1];

	bool isRegDestination, is16bitsOps;
	is16bitsOps = movOps & 1;
	isRegDestination = (movOps & 2);

	unsigned char rm = (argOps & 7);
	unsigned char reg = (argOps >> 3) & 7;
	unsigned char mod = (argOps >> 6) & 7;

	output << "mov ";
	bool registerMode = mod == 3;
	if (registerMode) {
		if (is16bitsOps) {
			src = isRegDestination ? registerMapping16bits[rm] : registerMapping16bits[reg];
			dst = isRegDestination ? registerMapping16bits[reg] : registerMapping16bits[rm];
		}
		else {
			src = isRegDestination ? registerMapping8bits[rm] : registerMapping8bits[reg];
			dst = isRegDestination ? registerMapping8bits[reg] : registerMapping8bits[rm];
		}
		output << dst << ", " << src;
	}
	else {
		// memory location
		char address[32] = {};
		address[0] = '[';
		strcat(address, registerToMemory[rm]);
		// offset value
		int16_t offset = 0;
		bool isDirectMemory = (mod == 0) && rm == 6;
		if (isDirectMemory) {
			address[0] = '[';
			address[1] = '\0';
			offset |= data[index + 3];
			offset = offset << 8;
			offset |= data[index + 2];
		}
		// fetch hi byte
		else if (mod == 2) {
			offset |= data[index + 3];
			offset = offset << 8;
			offset |= data[index + 2];
		}
		else if (mod == 1) {
			int8_t shortOffset = data[index + 2];
			offset = shortOffset;
		}

		if (offset) {
			char offsetStr[16] = {};
			sprintf(offsetStr, " + %d", offset);
			strcat(address, offsetStr);
		}
		strcat(address, "]");

		// load
		if (isRegDestination) {
			dst = is16bitsOps ? registerMapping16bits[reg] : registerMapping8bits[reg];
			src = address;
		}
		// store
		else {
			dst = address;
			src = is16bitsOps ? registerMapping16bits[reg] : registerMapping8bits[reg];
		}
		output << dst << ", " << src;
		// offset
		if (mod == 1) {
			index = index + 1;
		}
		if (mod == 2 || isDirectMemory) {
			index = index + 2;
		}
	}

	output << std::endl;
	// argops
	index++;
}



void decodeRegisterOp(const std::vector<unsigned char>& data, size_t& index, std::ofstream& output)
{
	const char* src = nullptr;
	const char* dst = nullptr;

	unsigned char movOps = data[index];
	unsigned char argOps = data[index + 1];

	bool isRegDestination, is16bitsOps;
	is16bitsOps = movOps & 1;
	isRegDestination = (movOps & 2);

	unsigned char rm = (argOps & 7);
	unsigned char reg = (argOps >> 3) & 7;
	unsigned char mod = (argOps >> 6) & 7;

	output << "mov ";
	bool registerMode = mod == 3;
	if (registerMode) {
		if (is16bitsOps) {
			src = isRegDestination ? registerMapping16bits[rm] : registerMapping16bits[reg];
			dst = isRegDestination ? registerMapping16bits[reg] : registerMapping16bits[rm];
		}
		else {
			src = isRegDestination ? registerMapping8bits[rm] : registerMapping8bits[reg];
			dst = isRegDestination ? registerMapping8bits[reg] : registerMapping8bits[rm];
		}
		output << dst << ", " << src;
	}
	else {
		// memory location
		char address[32] = {};
		address[0] = '[';
		strcat(address, registerToMemory[rm]);
		// offset value
		int16_t offset = 0;
		bool isDirectMemory = (mod == 0) && rm == 6;
		if (isDirectMemory) {
			address[0] = '[';
			address[1] = '\0';
			offset |= data[index + 3];
			offset = offset << 8;
			offset |= data[index + 2];
		}
		// fetch hi byte
		else if (mod == 2) {
			offset |= data[index + 3];
			offset = offset << 8;
			offset |= data[index + 2];
		}
		else if (mod == 1) {
			int8_t shortOffset = data[index + 2];
			offset = shortOffset;
		}

		if (offset) {
			char offsetStr[16] = {};
			sprintf(offsetStr, " + %d", offset);
			strcat(address, offsetStr);
		}
		strcat(address, "]");

		// load
		if (isRegDestination) {
			dst = is16bitsOps ? registerMapping16bits[reg] : registerMapping8bits[reg];
			src = address;
		}
		// store
		else {
			dst = address;
			src = is16bitsOps ? registerMapping16bits[reg] : registerMapping8bits[reg];
		}
		output << dst << ", " << src;
		// offset
		if (mod == 1) {
			index = index + 1;
		}
		if (mod == 2 || isDirectMemory) {
			index = index + 2;
		}
	}

	output << std::endl;
	// argops
	index++;
}












void decodeImmediateMove(const std::vector<unsigned char>& data, size_t& index, std::ofstream& output)
{
	const char* dst = nullptr;

	unsigned char movOps = data[index];
	bool is16bitsOps;
	is16bitsOps = movOps & 8; // 4th bits
	unsigned char reg = movOps & 7;
	dst = is16bitsOps ? registerMapping16bits[reg] : registerMapping8bits[reg];
	// hard coded constant
	int16_t offset = 0;
	// fetch hi byte
	if (is16bitsOps) {
		offset |= data[index + 2];
		offset = offset << 8;
		offset |= data[index + 1];
	}
	else {
		int8_t shortOffset = data[index + 1];
		offset = shortOffset;
	}
	// fetch low
	offset |= data[index + 1];
	char offsetStr[16] = {};
	sprintf(offsetStr, "%d", offset);

	output << "mov " << dst << ", " << offset;
	output << std::endl;

	if (is16bitsOps)
	{
		index++;
	}
	index++;
}

void decodeBytes(const std::vector<unsigned char>& data)
{
	std::ofstream output;
	output.open("G:\\Projects\\TutoAssembly\\AssemblyDecoder\\ressources\\result.asm", std::ios::binary);
	output << "bits 16" << std::endl;
	// MOV:       11  2 | 3 | 3,            IMEDIATE MOV:     1 3  
	// MOV: 100010DW MOD|REG|RM, mov AX, 12 IMEDIATE MOV: 1011WREG
	const char* operationStr[255];
	unsigned char movOperation = 34; // 100010
	unsigned char immediatemovOperation = 11; // 1011WREG
	unsigned char addOperation = 0;  // 000000
	unsigned char addCarryOps  = 1;  // 000001
	unsigned char subOperation = 10; // 001010
	unsigned char subCarryOps  = 11; // 001011
	unsigned char cmpOperation = 14; // 001110
	unsigned char cmpCarryOps  = 15; // 001111
	unsigned char immediateToRegisterOp = 32; // 100000

	operationStr[movOperation] = "mov";
	operationStr[immediatemovOperation] = "mov";
	operationStr[addOperation] = "add";
	operationStr[addCarryOps]  = "add";
	operationStr[subOperation] = "sub";
	operationStr[subCarryOps]  = "sub";
	operationStr[cmpOperation] = "cmp";
	operationStr[cmpCarryOps]  = "cmp";

	unsigned char jumpEqualOps = 116; // 01110100
	unsigned char jumpOnLessOps = 124; // 01111100
	unsigned char jumpOnLessOrEqualOps = 126; // 01111110
	unsigned char jumpOnNotAboveOrEqualOps = 114; // 01110010
	unsigned char jumpOnNotAboveOps = 118; // 01110110
	unsigned char jumpOnParityOps = 122; // 01111010
	unsigned char jumpOnOverflowOps = 112; // 01110000
	unsigned char jumpOnSignOps = 120; // 01111000
	unsigned char jumpOnNotEqualOps = 117; // 01110101
	unsigned char jumpOnGreaterOrEqualOps = 125; // 01111101
	unsigned char jumpOnGreaterOps = 127; // 01111111
	unsigned char jumpOnAboveOrEqualsOps = 115; // 01110011
	unsigned char jumpOnAboveOps = 119; // 01110111
	unsigned char jumpOnNotParityOps = 123; // 01111011
	unsigned char jumpOnNotOverflowOps = 113; // 01110001
	unsigned char jumpOnNotSignedOps = 121; // 01111001
	unsigned char loopOps = 226; // 11100010
	unsigned char loopWhileZeroOps = 225; // 11100001
	unsigned char loopWhileNotZeroOps = 224; // 11100000
	unsigned char jumpOnCxZeroOps = 227; // 11100011
	
	operationStr[jumpEqualOps] = "je";
	operationStr[jumpOnLessOps] = "jl";
	operationStr[jumpOnLessOrEqualOps] = "jle";
	operationStr[jumpOnNotAboveOrEqualOps] = "jb";
	operationStr[jumpOnNotAboveOps] = "jbe";
	operationStr[jumpOnParityOps] = "jp";
	operationStr[jumpOnOverflowOps] = "jo";
	operationStr[jumpOnSignOps] = "js";
	operationStr[jumpOnNotEqualOps] = "jne";
	operationStr[jumpOnGreaterOrEqualOps] = "jnl";
	operationStr[jumpOnGreaterOps] = "jg";
	operationStr[jumpOnAboveOrEqualsOps] = "jnb";
	operationStr[jumpOnAboveOps] = "ja";
	operationStr[jumpOnNotParityOps] = "jnp";
	operationStr[jumpOnNotOverflowOps] = "jno";
	operationStr[jumpOnNotSignedOps] = "jns";
	operationStr[loopOps] = "loop";
	operationStr[loopWhileZeroOps] = "loopz";
	operationStr[loopWhileNotZeroOps] = "loopnz";
	operationStr[jumpOnCxZeroOps] = "jcxz";

	const char* src = nullptr;
	const char* dst = nullptr;
	for (size_t i = 0; i < data.size(); )
	{
		unsigned char current = data[i];
		unsigned char currentOps = current >> 2;
		if ((currentOps) == movOperation)
		{
			decodeMove(data, i, output);
		}
		else if ((current >> 4) == immediatemovOperation)
		{
			decodeImmediateMove(data, i, output);
		}
		else
		{
			const char* src = nullptr;
			const char* dst = nullptr;
			i = i + 1;
			unsigned char argOps = data[i];
			unsigned char rm = (argOps & 7);
			unsigned char reg = (argOps >> 3) & 7;
			unsigned char mod = (argOps >> 6) & 7;

			bool isAddOps = (currentOps == addOperation);
			bool isSubOps = (currentOps == subOperation);
			bool isCmpOps = (currentOps == cmpOperation);
			bool isAddCarryOps = (currentOps == addCarryOps);
			bool isSubCarryOps = (currentOps == subCarryOps);
			bool isCmpCarryOps = (currentOps == cmpCarryOps);
			bool isImmediateOps = (currentOps == immediateToRegisterOp);

			bool isJumpOps = (current >= jumpOnOverflowOps) && (current <= jumpOnGreaterOps);
			bool isLoopOps = (current >= loopWhileNotZeroOps) && (current <= jumpOnCxZeroOps);

			if (isAddOps || isSubOps || isCmpOps)
			{
				const char* opsStr = operationStr[currentOps];
				bool isRegDestination, is16bitsOps;
				is16bitsOps = current & 1;
				isRegDestination = (current & 2);

				output << opsStr << " ";
				bool registerMode = mod == 3;
				if (registerMode) {
					if (is16bitsOps) {
						src = isRegDestination ? registerMapping16bits[rm] : registerMapping16bits[reg];
						dst = isRegDestination ? registerMapping16bits[reg] : registerMapping16bits[rm];
					}
					else {
						src = isRegDestination ? registerMapping8bits[rm] : registerMapping8bits[reg];
						dst = isRegDestination ? registerMapping8bits[reg] : registerMapping8bits[rm];
					}
					output << dst << ", " << src;
				}
				else {
					// memory location
					char address[32] = {};
					address[0] = '[';
					strcat(address, registerToMemory[rm]);
					// offset value
					int16_t offset = 0;
					bool isDirectMemory = (mod == 0) && rm == 6;
					if (isDirectMemory) {
						address[0] = '[';
						address[1] = '\0';
						offset |= data[i + 2];
						offset = offset << 8;
						offset |= data[i + 1];
					}
					// fetch hi byte
					else if (mod == 2) {
						offset |= data[i + 2];
						offset = offset << 8;
						offset |= data[i + 1];
					}
					else if (mod == 1) {
						int8_t shortOffset = data[i + 1];
						offset = shortOffset;
					}

					if (offset) {
						if (!isDirectMemory) {
							strcat(address, " + ");
						}
						char offsetStr[16] = {};
						sprintf(offsetStr, "%d", offset);
						strcat(address, offsetStr);
					}
					strcat(address, "]");

					// load
					if (isRegDestination) {
						dst = is16bitsOps ? registerMapping16bits[reg] : registerMapping8bits[reg];
						src = address;
					}
					// store
					else {
						dst = address;
						src = is16bitsOps ? registerMapping16bits[reg] : registerMapping8bits[reg];
					}
					output << dst << ", " << src;
					// offset
					if (mod == 1) {
						i = i + 1;
					}
					if (mod == 2 || isDirectMemory) {
						i = i + 2;
					}
				}
			}
			else if (isAddCarryOps || isSubCarryOps || isCmpCarryOps)
			{
				// nor arg ops
				i = i - 1;
				const char* opsStr = operationStr[currentOps];
				bool is16bitsOps;
				is16bitsOps = current & 1;
				const char* dst = is16bitsOps ? "ax" : "al";
				output << opsStr << " ";
				output << dst << ", ";

				int16_t offset = 0;
				// fetch hi byte
				if (is16bitsOps) {
					offset |= data[i + 2];
					offset = offset << 8;
					offset |= data[i + 1];
					i = i + 2;
				}
				else {
					int8_t shortOffset = data[i + 1];
					offset = shortOffset;
					i = i + 1;
				}
				char offsetStr[16] = {};
				sprintf(offsetStr, "%d", offset);
				output << offsetStr;
			}
			else if(isImmediateOps)
			{
				const char* regToOperationStr[8];
				regToOperationStr[0] = "add"; // 000
				regToOperationStr[5] = "sub"; // 101
				regToOperationStr[7] = "cmp"; // 111
				const char* opsStr = regToOperationStr[reg];
				bool is16bitsOps = current & 1;
				bool isSignedOperation = (current & 2);

				output << opsStr << " ";
				bool registerMode = mod == 3;
				if (registerMode) {
					dst = is16bitsOps ? registerMapping16bits[rm] : registerMapping8bits[rm];
				}
				else
				{
					char address[32] = {};
					if (is16bitsOps)
					{
						output << "word ";
					}
					else 
					{
						output << "byte ";
					}
					address[0] = '[';
					strcat(address, registerToMemory[rm]);
					// offset value
					int16_t offset = 0;
					bool isDirectMemory = (mod == 0) && rm == 6;
					if (isDirectMemory) {
						address[0] = '[';
						address[1] = '\0';
						offset |= data[i + 2];
						offset = offset << 8;
						offset |= data[i + 1];
					}
					// fetch hi byte
					else if (mod == 2) {
						offset |= data[i + 2];
						offset = offset << 8;
						offset |= data[i + 1];
					}
					else if (mod == 1) {
						int8_t shortOffset = data[i + 1];
						offset = shortOffset;
					}

					if (offset) {
						char offsetStr[16] = {};
						if (!isDirectMemory) {
							strcat(address, " + ");
						}
						if (isSignedOperation) {
							sprintf(offsetStr, "%d", offset);
						}
						else {
							sprintf(offsetStr, "%u", offset);
						}
						strcat(address, offsetStr);
					}
					strcat(address, "]");
					dst = address;

					if (mod == 1) {
						i = i + 1;
					}
					if (mod == 2 || isDirectMemory) {
						i = i + 2;
					}
				}

				int16_t immediate = 0;
				if (is16bitsOps && !isSignedOperation) {
					immediate |= data[i + 2];
					immediate = immediate << 8;
					immediate |= data[i + 1];
					i = i + 2;
				}
				//else if (mod == 1) {
				else {
					int8_t shortOffset = data[i + 1];
					immediate = shortOffset;
					i = i + 1;
				}
				output << dst << ", " << immediate;
			}
			else if (isJumpOps || isLoopOps)
			{
				const char* opsStr = operationStr[current];
				char offsetStr[16] = {};
				int8_t offset = 0;
				offset = data[i];
				sprintf(offsetStr, "%d", offset);
				output << opsStr << " " << offsetStr;
			}
			output << std::endl;
		}
		i++;
	}

	output.close();
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

	decodeBytes(fileData);
}