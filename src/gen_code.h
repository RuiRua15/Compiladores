#ifndef __GEN_CODE_H__
#define __GEN_CODE_H__

#include "ast.h"
#include "ir.h"

// the main function: converts an AST Stmt list into a TAC list
TAC *generate_code(Stmt *root);

#endif