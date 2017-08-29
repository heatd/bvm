#pragma once

#include <string>

#include <registers.h>

struct InterpreterStatus
{
	uintptr_t registers[NUM_REGS];
	uintptr_t *stack;
};

class BytecodeInterpreter
{
private:
	unsigned char *bytecode;
	unsigned char *interp_pointer;
	size_t mapping_length;
	InterpreterStatus *status;
public:
	BytecodeInterpreter(std::string filename);
	~BytecodeInterpreter();
	void Start();
	InterpreterStatus *GetStatus() {return status;}
	void DumpRegisters();
};
