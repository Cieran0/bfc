#include "bfc.h"
#include <fstream>

void WriteHeader(std::vector<Token> tokens, std::ofstream &file)
{
	bool hasRead = false;
	bool hasWrite = false;

	for (int i = 0; i < tokens.size(); i++)
	{
		if (tokens[i].type == TokenType::READ) { hasRead = true; }
		else if (tokens[i].type == TokenType::WRITE) { hasWrite = true; }
		if (hasRead && hasWrite) break;
	}

	file << "BITS 64                                 \n";
	file << "                                        \n";
	file << "section .data                           \n";
	file << "                                        \n";
	file << "section .bss                            \n";
	file << "    buf:    resb 1                      \n";
	file << "    mem:    resq 3750                   \n";
	file << "                                        \n";
	file << "section .text                           \n";
	file << "    global _start                       \n";
	file << "                                        \n";
	if (hasRead)
	{
	file << "    read:                               \n";
	file << "        xor     rax,rax                 \n";
	file << "        xor     rdi,rdi                 \n";
	file << "        mov     rsi,buf                 \n";
	file << "        mov     rdx, 1                  \n";
	file << "        syscall                         \n";
	file << "        mov     r10b, byte[buf]         \n";
	file << "        mov     byte[mem+r9], r10b      \n";
	file << "        ret                             \n";
	file << "                                        \n";
	}
	if (hasWrite)
	{
	file << "    write:                              \n";
	file << "        mov     rax,1                   \n";
	file << "        mov     rdx,1                   \n";
	file << "        mov     rdi,1                   \n";
	file << "        mov     r10b, byte[mem+r9]      \n";
	file << "        mov     byte [buf], r10b        \n";
	file << "        mov     rsi,buf                 \n";
	file << "        syscall                         \n";
	file << "        ret                             \n";
	file << "                                        \n";
	}
	file << "    check:                              \n";
	file << "        cmp     r9, 3750                \n";
	file << "        jl      c1                      \n";
	file << "        xor     r9,r9                   \n";
	file << "    c1:                                 \n";
	file << "        cmp     r9, 0                   \n";
	file << "        jnl     c2                      \n";
	file << "        mov     r9, 3749                \n";
	file << "    c2:                                 \n";
	file << "        ret                             \n";
	file << "                                        \n";
	file << "    _start:                             \n";
}

void Read(std::ofstream& file) { file <<			"        call    read               \n"; }
void Write(std::ofstream& file) { file <<			"        call    write              \n"; }
void Next(std::ofstream& file, int count) { file << "        add     r9, " << count << "\n"; file << "        call    check              \n"; }
void Prev(std::ofstream& file, int count) { file << "        sub     r9, " << count << "\n"; file << "        call    check              \n"; }
void Inc(std::ofstream& file, int count) { file <<  "        add     byte [mem + r9], " << count << "\n"; }
void Dec(std::ofstream& file, int count) { file <<  "        sub     byte [mem + r9], " << count << "\n"; }

void LoopStart(std::ofstream& file, int num)
{
	file << "        mov     al, byte [mem + r9]\n";
	file << "        cmp     al, 0              \n";
	file << "        je     LpE" << num << "\n";
	file << "     LpS" << num << ":\n";
}

void LoopEnd(std::ofstream& file, int num)
{
	file << "        mov     al, byte [mem + r9]\n";
	file << "        cmp     al, 0              \n";
	file << "        jne     LpS" << num << "\n";
	file << "     LpE" << num << ":\n";
}

void Compile(std::vector<Token> tokens, const char* outputPath)
{
	std::ofstream file;
	file.open(outputPath);
	WriteHeader(tokens, file);
	for (int i = 0; i < tokens.size(); i++)
	{
		switch (tokens[i].type) 
		{
			case TokenType::INC:
				Inc(file, tokens[i].count);
				break;
			case TokenType::DEC:
				Dec(file, tokens[i].count);
				break;
			case TokenType::NEXT:
				Next(file, tokens[i].count);
				break;
			case TokenType::PREV:
				Prev(file, tokens[i].count);
				break;
			case TokenType::READ:
				Read(file);
				break;
			case TokenType::WRITE:
				Write(file);
				break;
			case TokenType::LOOPSTART:
				LoopStart(file, tokens[i].count);
				break;
			case TokenType::LOOPEND:
				LoopEnd(file, tokens[i].count);
				break;
		}
	}
	file << "        mov     rax, 60                 \n";
	file << "        xor     rdi, rdi                \n";
	file << "        syscall                         \n";
	file.close();
}
