#include <iostream>
#include <string>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <bytecode.h>

#define PROGRAM_OPTIONS	""

void print_usage()
{
	std::cout << "Usage: bvm [ARGUMENTS] files ..." << std::endl;
}

int main(int argc, char **argv, char **envp)
{
	int c;
	if(argc < 2)
	{
		print_usage();
		return 1;
	}
	while((c = getopt(argc, argv, PROGRAM_OPTIONS)) != -1)
	{
		switch(c)
		{
			case '?':
			{
				print_usage();
				return 1;
			}
		}
	}

	if(optind < 0)
	{
		std::cout << "bvm: Missing filename." << std::endl;
		print_usage();
		return 1;
	}
	std::string filename(argv[optind]);

	BytecodeInterpreter bcode(filename);
	bcode.Start();
}
