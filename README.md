# Compiladores-2025.2

Implementação de um analisador léxico para a disciplina de Compiladores.

## Para compilar
### Análise léxica
gcc -I include src/main.c src/lexer.c src/symbol_table.c -o compilador.exe 

### Análise sintática
gcc -I include src/main_parser.c src/lexer.c src/symbol_table.c src/parser.c -o compilador.exe

## Para executar
./compilador.exe 
