#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/symbol_table.h"

No *tabela_simbolos = NULL;
int posicao_na_tabela;

int inserir_na_tabela(No **lista, char *lexema, enum tipo_token tipo){
    No *aux, *novo = malloc(sizeof(No));
    No *ultimo = NULL;
    int pos = 0;

    if(novo){
        strcpy(novo -> lex, lexema);
        novo -> tipo = tipo;
        novo -> proximo = NULL;

        if(*lista == NULL){
            novo -> pos = pos;
            *lista = novo;
        }
        else{
            aux = *lista;
            while(aux != NULL){
              if(strcmp(aux -> lex, lexema) == 0){
                int pos_atual = aux -> pos;
                free(novo);
                return pos_atual;
              }
              ultimo = aux;
              pos++;
              aux = aux -> proximo;  
            }
            novo -> pos = pos;
            ultimo -> proximo = novo;
        }

        return novo -> pos;
    }
    else{
        printf("Erro ao alocar memória!\n");
        return -1;
    }
}

void liberar_tabela(No **lista) {
    No *atual = *lista;
    No *prox;

    while (atual != NULL) {
        prox = atual->proximo;
        free(atual);
        atual = prox;
    }

    *lista = NULL;
}