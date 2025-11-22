#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#include "tokens.h"

typedef struct no{
  int pos;
  char lex[30];
  enum tipo_token tipo;
  struct no *proximo;
}No;

extern No *tabela_simbolos;

int inserir_na_tabela(No **lista, char *lexema, enum tipo_token tipo);
void liberar_tabela(No **lista);

#endif