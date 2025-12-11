#ifndef __MIPS_CODE_H__
#define __MIPS_CODE_H__

#include "ir.h"

// translates the TAC list into MIPS assembly and writes it to a file
void mips_generate(TAC *tac_head, const char *filename);

#endif