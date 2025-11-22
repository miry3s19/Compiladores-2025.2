#include <stdio.h>
#include <stdlib.h>
#include "../include/lexer.h"
#include "../include/symbol_table.h"

int main() {
  Token token;
  inicializar_lexer();  // ADICIONE ESTA LINHA
  code = readFile("./tests/programa.txt");
  do {
    token = proximo_token();
  } while (token.nome_token != EOF);
  free(code);
  liberar_tabela(&tabela_simbolos);
  return 0;  // ADICIONE ESTE RETURN
}