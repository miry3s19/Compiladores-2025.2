#include <stdio.h>
#include <stdlib.h>
#include "../include/parser.h"
#include "../include/lexer.h"

Token token_atual;

void LE_TOKEN() {
    do {
        token_atual = proximo_token();
    } while (token_atual.nome_token == SMALL_COMMENTARY || 
             token_atual.nome_token == COMMENTARY);
}

// Erros
void ERRO(int numero_erro) {
    printf("ERRO SINTÁTICO %d: ", numero_erro);
	switch(numero_erro) {
		case 1: 
			printf("O programa deve começar com a palavra 'início' e terminar com a palavra 'fim'. \n");
			break;
		case 2:
			printf("Caractere ';' esperado. \n");
			break;
		case 3:
			printf("Identificador era esperado. \n");
			break;
		case 4:
			printf("Um comando válido para a liguagem era esperado.\n");
			break;
		case 5:
			printf("Caractere '(' ou '=' esperados. \n");
			break;
		case 6:
			printf("Parênteses foram abertos ou fechados incorretamente. \n");
			break;
		case 7:
			printf("Chaves foram abertas ou fechadas incorretamente. \n");
			break;
		default: 
			printf("Erro sintático\n"); 
			break;
	}
	exit(numero_erro);
}

void programa() {
    if(token_atual.nome_token == BEGIN){
        LE_TOKEN();
        decls();
        comandos();
        if(token_atual.nome_token == END) {
            LE_TOKEN();
        } else { 
            ERRO(1);
        }
    } else {
        ERRO(1);
    }
}

void decls() {
    if (token_atual.nome_token == TYPE_INT || 
        token_atual.nome_token == TYPE_FLOAT || 
        token_atual.nome_token == TYPE_STRING) {
        decl();
        decls();
    } else {
        // ε-produção
        return;
    }
}

void decl() {
    tipo();
    if(token_atual.nome_token == ID) {
        LE_TOKEN();
        if(token_atual.nome_token == SEMICOLON) {
            LE_TOKEN();
        } else {
            ERRO(2);
        }
    } else {
        ERRO(3);
    }
}

void tipo() {
    if(token_atual.nome_token == TYPE_INT || 
       token_atual.nome_token == TYPE_FLOAT || 
       token_atual.nome_token == TYPE_STRING) {
        LE_TOKEN();
    } else {
        ERRO(4);
    }
}

void comandos() { 
    if(token_atual.nome_token == ID || token_atual.nome_token == READ || 
       token_atual.nome_token == PRINT || token_atual.nome_token == IF || 
       token_atual.nome_token == WHILE || token_atual.nome_token == LEFT_BRACKET) {
        comando();
        comandos();    
    } else {
        // ε-produção
        return;
    }
}  

void comando() {
    if (token_atual.nome_token == ID)  {
        identificador();
        if(token_atual.nome_token == SEMICOLON) {
            LE_TOKEN();
        } else {
            ERRO(2);
        }
    } else if(token_atual.nome_token == READ) {
        entrada();
        if(token_atual.nome_token == SEMICOLON) {
            LE_TOKEN();
        } else {
            ERRO(2);
        }
    } else if(token_atual.nome_token == PRINT) {
        saida();
        if(token_atual.nome_token == SEMICOLON) {
            LE_TOKEN();
        } else {
            ERRO(2);
        }
    } else if(token_atual.nome_token == IF) {
        if_stmt();
    } else if(token_atual.nome_token == WHILE) {
        while_stmt();
    } else if(token_atual.nome_token == LEFT_BRACKET) {
        bloco();
    } else {
        ERRO(4);
    }
}

void identificador() {
    if(token_atual.nome_token == ID) {
        LE_TOKEN();
        acao();
    } else {
        ERRO(3);
    }
}  

void acao() {
    if(token_atual.nome_token == ASSIGN) {
        atribuicao();
    } else if(token_atual.nome_token == LEFT_PARENTHESIS) {
        chamada();
    } else {
        ERRO(5);
    }
}  

void atribuicao() {
    if(token_atual.nome_token == ASSIGN) {
        LE_TOKEN();
        expr();
    } else {
        ERRO(5);
    }
}  

void chamada() {
    if(token_atual.nome_token == LEFT_PARENTHESIS) {
        LE_TOKEN();
        args();
        if(token_atual.nome_token == RIGHT_PARENTHESIS) {
            LE_TOKEN();
        } else {
            ERRO(6);
        }
    } else {
        ERRO(6);
    }
}  

void args() {
    if(token_atual.nome_token == ID || token_atual.nome_token == NUMBER ||
       token_atual.nome_token == STRING || token_atual.nome_token == LEFT_PARENTHESIS ||
       token_atual.nome_token == OP_SUB) {
        expr_list();
    } else {
        // ε-produção
        return;
    }
}

void expr_list() {
    expr();
    if(token_atual.nome_token == COMMA) {
        LE_TOKEN();
        expr_list();
    }
}  

void entrada() {
    if(token_atual.nome_token == READ) {
        LE_TOKEN();
        if(token_atual.nome_token == LEFT_PARENTHESIS) {
            LE_TOKEN();
            if(token_atual.nome_token == ID) {
                LE_TOKEN();
                if(token_atual.nome_token == RIGHT_PARENTHESIS) {
                    LE_TOKEN();
                } else {
                    ERRO(6);
                }
            } else {
                ERRO(3);
            } 
        } else {
            ERRO(6);
        }
    } else {
        ERRO(4);
    }
}  

void saida() {
    if(token_atual.nome_token == PRINT) {
        LE_TOKEN();
        if(token_atual.nome_token == LEFT_PARENTHESIS) {
            LE_TOKEN();
            expr();
            if(token_atual.nome_token == RIGHT_PARENTHESIS) {
                LE_TOKEN();
            } else {
                ERRO(6);
            } 
        } else {
            ERRO(6);
        }
    } else {
        ERRO(4);
    }
}  

void if_stmt() {
    if(token_atual.nome_token == IF) {
        LE_TOKEN();
        if(token_atual.nome_token == LEFT_PARENTHESIS) {
            LE_TOKEN();
            expr();
            if(token_atual.nome_token == RIGHT_PARENTHESIS) {
                LE_TOKEN();
                comando();
                else_opt();
            } else {
                ERRO(6);
            }
        } else {
            ERRO(6);
        }
    } else {
        ERRO(4);
    }
}

void else_opt() {
    if(token_atual.nome_token == ELSE) {
        LE_TOKEN();
        comando();
    } else {
        return;
    }
}

void while_stmt() {
    if(token_atual.nome_token == WHILE) {
        LE_TOKEN();
        if(token_atual.nome_token == LEFT_PARENTHESIS) {
            LE_TOKEN();
            expr();
            if(token_atual.nome_token == RIGHT_PARENTHESIS) {
                LE_TOKEN();
                comando();
            } else {
                ERRO(6);
            } 
        } else {
            ERRO(6);
        }
    } else {
        ERRO(4);
    }
}  

void bloco() {
    if(token_atual.nome_token == LEFT_BRACKET) {
        LE_TOKEN();
        comandos();
        if(token_atual.nome_token == RIGHT_BRACKET) {
            LE_TOKEN();
        } else {
            ERRO(7);
        }
    } else {
        ERRO(7);
    }
}  

void expr() {
    term();
    expr_L();
}  

void expr_L() {
    if(token_atual.nome_token == OP_SUM || token_atual.nome_token == OP_SUB) {
        LE_TOKEN();
        term();
        expr_L();
    } else {
        return;
    }   
}  

void term() {
    factor();
    term_L();
}  

void term_L() {
    if(token_atual.nome_token == OP_MUL || token_atual.nome_token == OP_DIV) {
        LE_TOKEN();
        factor();
        term_L();
    } else {
        return;
    }   
}  

void factor() {
    if(token_atual.nome_token == ID) {
        LE_TOKEN();
    } else if(token_atual.nome_token == NUMBER) {
        LE_TOKEN();
    } else if(token_atual.nome_token == STRING) {
        LE_TOKEN();
    } else if(token_atual.nome_token == LEFT_PARENTHESIS) {
        LE_TOKEN();
        expr();
        if(token_atual.nome_token == RIGHT_PARENTHESIS) {
            LE_TOKEN();
        } else {
            ERRO(6);
        }
    } else if(token_atual.nome_token == OP_SUB) {
        LE_TOKEN();
        factor();
    } else {
        ERRO(4);
    }   
}