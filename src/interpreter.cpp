#include <stdexcept>
#include <iostream>
#include <cstdio>

#include <string.h>
#include <bytecode.h>
#include <opcode.h>
#include <native.h>

BytecodeInterpreter::BytecodeInterpreter(std::string filename)
{
	bytecode = interp_pointer = Native::MapWholeFile(filename, &mapping_length);
	if(!bytecode)
		throw std::runtime_error(std::string("Out of memory while mapping the file\n"));
	status = new InterpreterStatus;
	memset(status, 0, sizeof(InterpreterStatus));
}
BytecodeInterpreter::~BytecodeInterpreter()
{
	Native::UnmapWholeFile(bytecode, mapping_length);
}
void BytecodeInterpreter::DumpRegisters()
{
	std::printf("Register dump:\nIP: %016lx\n", interp_pointer);
	for(int i = 0; i < NUM_REGS; i++)
	{
		std::printf("R%d: %016lx\n", i, status->registers[i]);
	}
	std::printf("Stack: %016lx\n", status->stack);
	std::printf("8 byte dump of *IP: ");
	for(int i = 0; i < 8; i++)
	{
		std::printf("%02x ", *(interp_pointer + i));
	}
	std::printf("\n");
}
void BytecodeInterpreter::Start()
{
	size_t size = mapping_length;
	Opcode::OpcodeObject *opcode = new Opcode::OpcodeObject(NULL, 0);

	while(1)
	{
		try
		{
			opcode = Opcode::Decode(interp_pointer, opcode);
			/* If execute() returns NULL, we returned from the outermost stack frame */
			if(!(interp_pointer = opcode->execute(this)))
				return;
		}
		catch(std::exception &e)
		{
			std::cerr << "bvm: BytecodeInterpreter: Exception: " << e.what() << std::endl;
			DumpRegisters();
			return;
		}
	}
}
