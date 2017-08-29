#pragma once

#include <stddef.h>
#include <bytecode.h>

namespace Opcode
{

enum
{
	OPCODE_NOP = 0,
	OPCODE_ADD,
	OPCODE_MOV,
	OPCODE_LOAD,
	OPCODE_STORE,
	OPCODE_SUB,
	OPCODE_MUL,
	OPCODE_DIV,
	OPCODE_CALL,
	OPCODE_JMP
} Opcode;

class OpcodeObject
{
protected:
	unsigned char *opcode_location;
	size_t opcode_length;
public:
	OpcodeObject(unsigned char *ptr, size_t length) : opcode_location(ptr), opcode_length(length)
	{}
	/* Returns the new IP */
	virtual unsigned char *execute(BytecodeInterpreter *status)
	{
		return opcode_location;
	}
};

class OpcodeNop : public OpcodeObject
{
public:
	OpcodeNop(unsigned char *ptr) : OpcodeObject(ptr, 1){}
	unsigned char *execute(BytecodeInterpreter *status);
};

class OpcodeAdd : public OpcodeObject
{
public:
	OpcodeAdd(unsigned char *ptr) : OpcodeObject(ptr, 0){}
	unsigned char *execute(BytecodeInterpreter *bcode);
};

struct opcode_add
{
	unsigned char opcode;
	/* The add instruction takes two registers as an operands
	 * and stores the result in the first one
	*/
	unsigned char operand0;
	unsigned char operand1;
};

class OpcodeMov : public OpcodeObject
{
public:
	OpcodeMov(unsigned char *ptr) : OpcodeObject(ptr, 0) {}
	unsigned char *execute(BytecodeInterpreter *bcode);
};

struct opcode_mov
{
	unsigned char opcode;
	unsigned char reg;

	#define LOAD_SIZE_8	0
	#define LOAD_SIZE_16	1
	#define LOAD_SIZE_32	2
	#define LOAD_SIZE_64	3

	unsigned char size; /* Uses the above defines */

	union
	{
		uint8_t byte;
		uint16_t word;
		uint32_t dword;
		uint64_t qword;
	} opcode_val;
} __attribute__((packed));

OpcodeObject *Decode(unsigned char *, OpcodeObject *);

};
