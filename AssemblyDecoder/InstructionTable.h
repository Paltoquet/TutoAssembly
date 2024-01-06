
#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <map>

typedef enum : uint32_t {
	UNKNOWNOPS = 0,
	IMMEDIATEOPS, // specific code for immediate move, add, sub, cmp
	MOV,
	ADD,
	SUB,
	CMP,
	JUMP,
	JNZ,
	JE,
	JL,
	JLE,
	JB,
	JBE,
	JP,
	JO,
	JS,
	JNE,
	JNL,
	JG,
	JNB,
	JA,
	JNP,
	JNO,
	JNS,
	LOOP,
	LOOPZ,
	LOOPNZ,
	JCXZ,
} OperationType;

typedef enum : uint32_t
{
	UNKNOWNREG = 0,
	AL,
	AH,
	AX,
	BL,
	BH,
	BX,
	CL,
	CH,
	CX,
	DL,
	DH,
	DX,
	DI,
	SI,
	BP,
	SP,
} Register;

struct Offset
{
	Register reg;
	int32_t number;
};

struct Location
{
	Register reg;
	Offset offset;
};

struct Operation
{
	OperationType op;
	Location dst;
	Location src;
	int16_t immediate;
	bool hasByteKeyword;
	bool hasWordKeyword;
};

struct OperationDescription
{
	OperationType op;
	bool hasTwoArguments; // move, add, sub, cmp
	bool hasData; // immediate: mov, add, sub, cmp, ... 
	bool useIncrement; // jne, loop, jp, ...
	Register defaultRegister; // accumulator: add, sub, cmp
};


// could be vector, looks better this way
extern std::map<Register, const char*> registerToString;
extern std::map<OperationType, const char*> operationToString;
extern OperationDescription instructionTable[255];

void initOperationTable();
void printLocation(const Location& location, std::ofstream& output);
void printOperation(const Operation& operation, std::ofstream& output);