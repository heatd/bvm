#include <stdexcept>
#include <opcode.h>
#include <iostream>

namespace Opcode
{

unsigned char *OpcodeNop::execute(BytecodeInterpreter *status)
{
	return opcode_location + opcode_length;
}

unsigned char *OpcodeAdd::execute(BytecodeInterpreter *bcode)
{
	InterpreterStatus *status = bcode->GetStatus();

	struct opcode_add *opc = (struct opcode_add *) opcode_location;
	
	if(opc->operand0 > NUM_REGS)
		throw std::runtime_error(std::string("Add instruction: Invalid register"));
	if(opc->operand1 > NUM_REGS)
		throw std::runtime_error(std::string("Add instruction: Invalid register"));
	uintptr_t o0 = status->registers[opc->operand0]; 
	uintptr_t o1 = status->registers[opc->operand1];

	uintptr_t result = o0 + o1;

	status->registers[opc->operand0] = result;
	return opcode_location + sizeof(struct opcode_add);
}

unsigned char *OpcodeMov::execute(BytecodeInterpreter *bcode)
{
	InterpreterStatus *status = bcode->GetStatus();
	
	struct opcode_mov *opc = (struct opcode_mov *) opcode_location;

	if(opc->reg > NUM_REGS)
		throw std::runtime_error(std::string("Load instruction: Invalid register"));

	int idx = opc->reg;
	/* The size of 'store' is 3 bytes + data size */
	size_t opcode_size = 3;
	switch(opc->size)
	{
		case LOAD_SIZE_8:
		{
			status->registers[idx] = (status->registers[idx] & 0xffffffffffffff00)
						 | opc->opcode_val.byte;
			opcode_size++;
			break;
		}
		case LOAD_SIZE_16:
		{
			status->registers[idx] = (status->registers[idx] & 0xffffffffffff0000)
						 | opc->opcode_val.word;
			opcode_size += 2;
			break;
		}
		case LOAD_SIZE_32:
		{
			status->registers[idx] = (status->registers[idx] & 0xffffffff00000000)
						 | opc->opcode_val.dword;
			opcode_size += 4;
			break;
		}
		case LOAD_SIZE_64:
		{
			status->registers[idx] = opc->opcode_val.qword;
			opcode_size += 8;
			break;
		}
		default:
		{
			std::cerr << "Invalid size " << (int) opc->size << std::endl;
			throw std::runtime_error(std::string("Load instruction: Invalid size"));
		}
	}
	return opcode_location + opcode_size;
}
OpcodeObject *Decode(unsigned char *ptr, OpcodeObject *obj)
{
	switch(*ptr)
	{
		case OPCODE_NOP:
		{
			return new (obj) OpcodeNop(ptr);
		}
		case OPCODE_ADD:
		{
			return new (obj) OpcodeAdd(ptr);
		}
		case OPCODE_MOV:
		{
			return new (obj) OpcodeMov(ptr);
		}
		default:
			throw std::runtime_error(std::string("Unknown opcode\n"));
	}
	return nullptr;
}

};
