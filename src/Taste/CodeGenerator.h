#if !defined(TASTE_CODEGENERATOR_H__)
#define TASTE_CODEGENERATOR_H__

#include "Scanner.h"
#include <stdlib.h>
#include <stdio.h>

namespace Taste {

class CodeGenerator
{
public:
	// opcodes
	int
	  ADD,  SUB,   MUL,   DIV,   EQU,  LSS, GTR, NEG,
	  LOAD, LOADG, STO,   STOG,  CONST,
	  CALL, RET,   ENTER, LEAVE,
	  JMP,  FJMP,  READ,  WRITE;

#define OPCODE_SIZE 21
	wchar_t* opcode[OPCODE_SIZE];
        //memset(opcode, 0, OPCODE_SIZE * sizeof(wchar_t*));

	int progStart;		// address of first instruction of main program
	int pc;				// program counter
	char *code;

	// data for Interpret
	int *globals;
	int *stack;
	int top;	// top of stack
	int bp;		// base pointer


	CodeGenerator() {
		// opcodes
		ADD  =  0; SUB   =  1; MUL   =  2; DIV   =  3; EQU   =  4; LSS = 5; GTR = 6; NEG = 7;
		LOAD =  8; LOADG =  9; STO   = 10; STOG  = 11; CONST = 12;
		CALL = 13; RET   = 14; ENTER = 15; LEAVE = 16;
		JMP  = 17; FJMP  = 18; READ  = 19; WRITE = 20;

		opcode[ 0] = coco_string_create("ADD  ");
		opcode[ 1] = coco_string_create("SUB  ");
		opcode[ 2] = coco_string_create("MUL  ");
		opcode[ 3] = coco_string_create("DIV  ");
		opcode[ 4] = coco_string_create("EQU  ");
		opcode[ 5] = coco_string_create("LSS  ");
		opcode[ 6] = coco_string_create("GTR  ");
		opcode[ 7] = coco_string_create("NEG  ");
		opcode[ 8] = coco_string_create("LOAD ");
		opcode[ 9] = coco_string_create("LOADG");
		opcode[10] = coco_string_create("STO  ");
		opcode[11] = coco_string_create("STOG ");
		opcode[12] = coco_string_create("CONST");
		opcode[13] = coco_string_create("CALL ");
		opcode[14] = coco_string_create("RET  ");
		opcode[15] = coco_string_create("ENTER");
		opcode[16] = coco_string_create("LEAVE");
		opcode[17] = coco_string_create("JMP  ");
		opcode[18] = coco_string_create("FJMP ");
		opcode[19] = coco_string_create("READ ");
		opcode[20] = coco_string_create("WRITE");

#define CODE_SIZE 3000
#define GLOBALS_SIZE 100
		code    = new char[CODE_SIZE];
                memset(code, 0, CODE_SIZE);
		globals = new int[GLOBALS_SIZE];
                memset(globals, 0, GLOBALS_SIZE * sizeof(*globals));
		stack   = new int[GLOBALS_SIZE];
                memset(stack, 0, GLOBALS_SIZE * sizeof(*stack));

		progStart = 0;

		pc = 1;
	}

	~CodeGenerator() {
		coco_string_delete(opcode[ 0]);
		coco_string_delete(opcode[ 1]);
		coco_string_delete(opcode[ 2]);
		coco_string_delete(opcode[ 3]);
		coco_string_delete(opcode[ 4]);
		coco_string_delete(opcode[ 5]);
		coco_string_delete(opcode[ 6]);
		coco_string_delete(opcode[ 7]);
		coco_string_delete(opcode[ 8]);
		coco_string_delete(opcode[ 9]);
		coco_string_delete(opcode[10]);
		coco_string_delete(opcode[11]);
		coco_string_delete(opcode[12]);
		coco_string_delete(opcode[13]);
		coco_string_delete(opcode[14]);
		coco_string_delete(opcode[15]);
		coco_string_delete(opcode[16]);
		coco_string_delete(opcode[17]);
		coco_string_delete(opcode[18]);
		coco_string_delete(opcode[19]);
		coco_string_delete(opcode[20]);
		delete[] code;
		delete[] globals;
		delete[] stack;
	}

	//----- code generation methods -----

	void Emit (int op) {
		//printf("Emit : %d\n", op);
		code[pc++] = (char)op;
	}

	void Emit (int op, int val) {
		//printf("Emit : %d, %d\n", op, val);
		Emit(op); Emit(val>>8); Emit(val);
	}

	void Patch (int adr, int val) {
		code[adr] = (char)(val>>8); code[adr+1] = (char)val;
	}

	void Decode() {
		int maxPc = pc;
		pc = 1;
		while (pc < maxPc) {
			int code = Next();
			printf("%3d: %" _SFMT " ", pc-1, opcode[code]);
			if (code == LOAD || code == LOADG || code == CONST || code == STO || code == STOG ||
				code == CALL || code == ENTER || code == JMP   || code == FJMP)
					printf("%d\n", Next2());
			else
			if (code == ADD  || code == SUB || code == MUL || code == DIV || code == NEG ||
				code == EQU  || code == LSS || code == GTR || code == RET || code == LEAVE ||
				code == READ || code == WRITE)
					printf("\n");
		}
	}

	//----- interpreter methods -----

	int Next () {
		return code[pc++];
	}

	int Next2 () {
		int x,y;
		x = code[pc++]; y = code[pc++];
		return (x << 8) + y;
	}

	int Int (bool b) {
		if (b) return 1; else return 0;
	}

	void Push (int val) {
		//printf("Push : %d\n", top);
		stack[top++] = val;
	}

	int Pop() {
		//printf("Pop : %d\n", top);
		return stack[--top];
	}

	int ReadInt(FILE* s) {
		int sign;
		char ch;
		do {fscanf(s, "%c", &ch);} while (!((ch >= '0' && ch <= '9') || ch == '-'));

		if (ch == '-') {sign = -1; fscanf(s, "%c", &ch);} else sign = 1;
		int n = 0;
		while (ch >= '0' && ch <= '9') {
			n = 10 * n + (ch - '0');
			if (fscanf(s, "%c", &ch) <= 0)
				break;
		}
		return n * sign;
	}

	void Interpret (const char* data) {
		int val;
		FILE* s;
		if ((s = fopen(data, "r")) == NULL) {
			printf("--- Error accessing file %s\n", (char*)data);
			exit(1);
		}
		printf("\n");
		pc = progStart; stack[0] = 0; top = 1; bp = 0;
		for (;;) {
			int nxt = Next();
			if (nxt == CONST)
				Push(Next2());
			else if (nxt == LOAD)
				Push(stack[bp+Next2()]);
			else if (nxt == LOADG)
				Push(globals[Next2()]);
			else if (nxt == STO)
				stack[bp+Next2()] = Pop();
			else if (nxt == STOG)
				globals[Next2()] = Pop();
			else if (nxt == ADD)
				Push(Pop()+Pop());
			else if (nxt == SUB)
				Push(-Pop()+Pop());
			else if (nxt == DIV)
				{val = Pop(); Push(Pop()/val);}
			else if (nxt == MUL)
				Push(Pop()*Pop());
			else if (nxt == NEG)
				Push(-Pop());
			else if (nxt == EQU)
				Push(Int(Pop()==Pop()));
			else if (nxt == LSS)
				Push(Int(Pop()>Pop()));
			else if (nxt == GTR)
				Push(Int(Pop()<Pop()));
			else if (nxt == JMP)
				pc = Next2();
			else if (nxt == FJMP)
				{ val = Next2(); if (Pop()==0) pc = val;}
			else if (nxt == READ)
				{val = ReadInt(s); Push(val);}
			else if (nxt == WRITE)
				printf("%d\n", Pop());
			else if (nxt == CALL)
				{Push(pc+2); pc = Next2();}
			else if (nxt == RET)
				{pc = Pop(); if (pc == 0) {fclose(s); return;}}
			else if (nxt == ENTER)
				{Push(bp); bp = top; top = top + Next2();}
			else if (nxt == LEAVE)
				{top = bp; bp = Pop();}
			else {
				printf("illegal opcode\n");
				exit(1);
			}
		}
		fclose(s);
	}

};

}; // namespace

#endif // !defined(TASTE_CODEGENERATOR_H__)
