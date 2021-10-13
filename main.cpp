#include "bfc.h"
#include <fstream>
#include <iostream>

int main(int argc, char** argv) 
{
	if (argc < 2)
	{
		std::exit(0);
	}

	bool keepFiles = false;
	int filePos = 1;

	if (argc > 2) if (argv[1] == "-k") { keepFiles = true; filePos++;}

	std::string filePath = argv[filePos];
	std::string outputPath = filePath.substr(0, filePath.find_last_of('.'));

	std::ifstream file(filePath, std::ifstream::binary);
	if (!file.good()) 
	{
		std::cout << "File " << argv[filePos] << " does not exist";
		std::exit(0);
	}

	file.seekg(0, file.end);
	int length = file.tellg();
	file.seekg(0, file.beg);

	char* input = new char[length];
	file.read(input, length);

	Compile(Condense(Tokenise(input)),(outputPath + ".asm").c_str());

	std::string nasmCommand = "nasm -f elf64 " + outputPath + ".asm";
	std::string ldCommand = "ld -o " + outputPath + " " + outputPath + ".o";

	system(nasmCommand.c_str());
	system(ldCommand.c_str());

	remove((outputPath + ".o").c_str());
	remove((outputPath + ".asm").c_str());
	return 0;
}

