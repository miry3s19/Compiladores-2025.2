#ifndef LEXER_H
#define LEXER_H

#include "tokens.h"

extern char *code;
unsigned char *readFile(char *fileName);
void inicializar_lexer();
Token proximo_token();

#endif