#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

// Procedimentos construídos
void programa();
void decls();
void decl();
void tipo();
void comandos();
void comando();
void identificador();
void acao();
void atribuicao();
void chamada();
void args();
void expr_list();
void entrada();
void saida();
void if_stmt();
void else_opt();
void while_stmt();
void bloco();
void expr();
void expr_L();
void term();
void term_L();
void factor();

//Funções auxiliares
void LE_TOKEN();
void ERRO(int numero_erro);

// Variável global para o token atual
extern Token token_atual;

#endif