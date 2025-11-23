#include <stdio.h>
#include <stdlib.h>
#include "../include/lexer.h"
#include "../include/symbol_table.h"
#include "../include/parser.h"

int main() {
    inicializar_lexer();
    code = readFile("./tests/caso_valido.txt");
    
    LE_TOKEN();
    
    programa();
    
    printf("Análise sintática concluída com SUCESSO!\\n");
    
    free(code);
    liberar_tabela(&tabela_simbolos);
    return 0;
}