
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//nome_tokens

enum tipo_token {
    ESTADO_INICIAL = 256,
    BEGIN, //inicio 
    ID, //id 
    NUMBER,
    NUMBER_FLOAT,
    STRING,
    SMALL_COMMENTARY,
    COMMENTARY,
    TYPE_INT, //int
    TYPE_FLOAT, //float
    TYPE_STRING, //string
    IF, //if
    ELSE, //else
    WHILE, //while
    READ, //read
    PRINT, //print
    SEMICOLON,//;
    ASSIGN, // =
    LEFT_PARENTHESIS, // (
    RIGHT_PARENTHESIS,// )
    LEFT_BRACKET, // {
    RIGHT_BRACKET, // }
    COMMA, // ,
    OP_SUM, // +
    OP_SUB, // -
    OP_MUL, // *
    OP_DIV, // /
    END // fim
};

enum atributos {
    INT = 282,
    FLOAT,
};

typedef struct Token {
  enum tipo_token nome_token;
  int atributo;
} Token;

//tabela de símbolos

typedef struct no{
  int pos;
  char lex[30];
  enum tipo_token tipo;
  struct no *proximo;
}No;

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

// variáveis globais

int estado = ESTADO_INICIAL;
int cont_sim_lido = 0;
char *code;

//leitura de arquivo

unsigned char *readFile(char *fileName) {
  FILE *file = fopen(fileName, "r");
  char *code;
  int n = 0;
  int c;

  if (!file) {
    fprintf(stderr, "Erro ao ler o arquivo: %s.\n", fileName);
    exit(3);
  }

  fseek(file, 0, SEEK_END);
  long f_size = ftell(file);
  fseek(file, 0, SEEK_SET);

  code = (unsigned char *)malloc((f_size + 1) * sizeof(char));

  while ((c = fgetc(file)) != EOF) {
    code[n++] = (unsigned char)c;
  }
  code[n] = '\0';
  return code;
}

void falhar(int erro) { //recebe o número do erro como parâmetro, encerra o programa quando chamada

  switch (erro) {

    case 1:
      printf("ERRO: caracter não pertercente a linguagem!\n");
      break;

    case 2:
      printf("ERRO: número inválido!\n");
      break;

    case 3:
      printf("ERRO: string não foi fechada!\n");
      break;
    
    case 4:
      printf("ERRO: comentário não foi fechado!\n");
      break;  

  }
  exit(-1);
}

Token proximo_token() {
  Token token;
  char c;
  while (cont_sim_lido < strlen(code) && code[cont_sim_lido] != '\0') {
    switch (estado) {
        
      case ESTADO_INICIAL:

            c = code[cont_sim_lido];
            if ((c == ' ') || (c == '\n')) {
              estado = ESTADO_INICIAL;
              cont_sim_lido++;
            }
            
            //ids e palavras reservadas(verificadas dentro de ID)

            else if (isalpha(c)){
                estado = ID;
            }
            
            //strings

            else if (c == '"'){
              estado = STRING;
            }

            //números

            else if (isdigit(c)){
              estado = NUMBER;
            }

            //caracteres únicos

            else if (c == ';') {
              estado = SEMICOLON;
            }
            else if (c == '=') {
              estado = ASSIGN;
            }
            else if (c == '(') {
              estado = LEFT_PARENTHESIS;
            }
            else if (c == ')') {
              estado = RIGHT_PARENTHESIS;
            }
            else if (c == '{') {
              estado = LEFT_BRACKET;
            }
            else if (c == '}') {
              estado = RIGHT_BRACKET;
            }
            else if (c == ',') {
              estado = COMMA;
            }
            else if (c == '+') {
              estado = OP_SUM;
            }
            else if (c == '-') {
              estado = OP_SUB;
            }
            else if (c == '*') {
              estado = OP_MUL;
            }
            else if (c == '/') {
              estado = OP_DIV;
            }
            else 
              falhar(1);
            break;
      
      case SMALL_COMMENTARY:
            c = code[cont_sim_lido];

            while (cont_sim_lido < strlen(code) && code[cont_sim_lido] != '\0' && code[cont_sim_lido] != '\n'){
              
              if(code[cont_sim_lido] == '[' && code[cont_sim_lido + 1] == '['){
                estado = COMMENTARY;
                cont_sim_lido += 2;
                break;
              }
              cont_sim_lido++;
              c = code[cont_sim_lido];
            }
            if (estado == SMALL_COMMENTARY){
              printf("<SMALL_COMMENTARY, >\n");
              token.nome_token = SMALL_COMMENTARY;
              token.atributo = -1;
              estado = ESTADO_INICIAL;
              return(token);
            }

            break;
      
      case COMMENTARY:
            while (cont_sim_lido + 1 < strlen(code) && code[cont_sim_lido] != '\0') {
                if (code[cont_sim_lido] == ']' && code[cont_sim_lido + 1] == ']') {
                    cont_sim_lido += 2;  // Pula "]]"
                    printf("<COMMENTARY, >\n");
                    token.nome_token = COMMENTARY;
                    token.atributo = -1;
                    estado = ESTADO_INICIAL;
                    return(token);
                }
                cont_sim_lido++;
            }
            falhar(4);
            break;
            
      case NUMBER:
            c = code[cont_sim_lido];

            while (cont_sim_lido < strlen(code) && code[cont_sim_lido] != '\0' && c != ' ' && isdigit(c)){
              cont_sim_lido++;
              c = code[cont_sim_lido];

              if(c == '.') {
                estado = NUMBER_FLOAT;
                break;
              }
            }

            if(estado == NUMBER){
              if(isdigit(c)){
                cont_sim_lido++;
              }
              printf("<NUMBER, INT>\n");
              token.nome_token = NUMBER;
              token.atributo = INT;
              estado = ESTADO_INICIAL;
              return(token);
            }

            break;

      case NUMBER_FLOAT:
            cont_sim_lido++;
            c = code[cont_sim_lido];
            
            if (!isdigit(c)){
              falhar(2);
            }

            while (cont_sim_lido < strlen(code) && code[cont_sim_lido] != '\0' && c != ' ' && isdigit(c)){
              cont_sim_lido++;
              c = code[cont_sim_lido];
            }

            if(isdigit(c)){
                cont_sim_lido++;
            }
            printf("<NUMBER, FLOAT>\n");
            token.nome_token = NUMBER;
            token.atributo = FLOAT;
            estado = ESTADO_INICIAL;
            return(token);
            
            break;
      
      case STRING:
            cont_sim_lido++;
            c = code[cont_sim_lido];

            while (cont_sim_lido < strlen(code) && code[cont_sim_lido] != '\0' && c != '"'){
              cont_sim_lido++;
              c = code[cont_sim_lido];
            }

            if (cont_sim_lido >= strlen(code) || code[cont_sim_lido] == '\0'){
              falhar(3);
            }
            else{
              cont_sim_lido++;
              printf("<STRING, >\n");
              token.nome_token = STRING;
              token.atributo = -1;
              estado = ESTADO_INICIAL;
              return(token);
            }
            
      break;

      case ID:
            char lexema[30];
            int pos = 0;
            
            lexema[pos] = c;
            cont_sim_lido++;
            pos++;
            
            while (cont_sim_lido < strlen(code) && code[cont_sim_lido] != '\0' && pos < 29){
              
                c = code[cont_sim_lido];

                if (c == ' ' || c == '\n' || (!isalnum(c) && c != '_')){
                  break;
                }
                
                lexema[pos] = c;
                pos++;
                cont_sim_lido++;
                
            }

            lexema[pos] = '\0';

            if(strcmp(lexema, "inicio") == 0){
              estado = BEGIN;
              cont_sim_lido--;
            }
            else if(strcmp(lexema, "fim") == 0){
              estado = END;
              cont_sim_lido--;
            }
            else if(strcmp(lexema, "if") == 0){
              estado = IF;
              cont_sim_lido--;
            }
            else if(strcmp(lexema, "else") == 0){
              estado = ELSE;
              cont_sim_lido--;
            }
            else if(strcmp(lexema, "while") == 0){
              estado = WHILE;
              cont_sim_lido--;
            }
            else if(strcmp(lexema, "read") == 0){
              estado = READ;
              cont_sim_lido--;
            }
            else if(strcmp(lexema, "print") == 0){
              estado = PRINT;
              cont_sim_lido--;
            }
            else if(strcmp(lexema, "int") == 0){
              estado = TYPE_INT;
              cont_sim_lido--;
            }
            else if(strcmp(lexema, "float") == 0){
              estado = TYPE_FLOAT;
              cont_sim_lido--;
            }
            else if(strcmp(lexema, "string") == 0){
              estado = TYPE_STRING;
              cont_sim_lido--;
            }
            else{
              posicao_na_tabela = inserir_na_tabela(&tabela_simbolos, lexema, ID);
              printf("<ID, %d>\n", posicao_na_tabela);
              token.nome_token = ID;
              token.atributo = posicao_na_tabela;
              estado = ESTADO_INICIAL;
              return(token);
            }

        break;

        case BEGIN:
            cont_sim_lido++;
            printf("<inicio, >\n");
            token.nome_token = BEGIN;
            token.atributo = -1;
            estado = ESTADO_INICIAL;
            return(token);
            break;
        
        case END: 
            cont_sim_lido++;
            printf("<fim, >\n");
            token.nome_token = END;
            token.atributo = -1;
            estado = ESTADO_INICIAL;
            return(token);
            break;

        case IF: 
            cont_sim_lido++;
            printf("<if, >\n");
            token.nome_token = IF;
            token.atributo = -1;
            estado = ESTADO_INICIAL;
            return(token);
            break;
        
        case TYPE_INT: 
            cont_sim_lido++;
            printf("<int, >\n");
            token.nome_token = TYPE_INT;
            token.atributo = -1;
            estado = ESTADO_INICIAL;
            return(token);
            break;

        case ELSE:
            cont_sim_lido++;
            printf("<else, >\n");
            token.nome_token = ELSE;
            token.atributo = -1;
            estado = ESTADO_INICIAL;
            return(token);
            break;
        
        case WHILE:
            cont_sim_lido++;
            printf("<while, >\n");
            token.nome_token = WHILE;
            token.atributo = -1;
            estado = ESTADO_INICIAL;
            return(token);
            break;

        case READ:
            cont_sim_lido++;
            printf("<read, >\n");
            token.nome_token = READ;
            token.atributo = -1;
            estado = ESTADO_INICIAL;
            return(token);
            break;
        
        case PRINT:
            cont_sim_lido++;
            printf("<print, >\n");
            token.nome_token = PRINT;
            token.atributo = -1;
            estado = ESTADO_INICIAL;
            return(token);
            break;
        
        case TYPE_FLOAT:
            cont_sim_lido++;
            printf("<float, >\n");
            token.nome_token = TYPE_FLOAT;
            token.atributo = -1;
            estado = ESTADO_INICIAL;
            return(token);
            break;

        case TYPE_STRING:
            cont_sim_lido++;
            printf("<string, >\n");
            token.nome_token = TYPE_STRING;
            token.atributo = -1;
            estado = ESTADO_INICIAL;
            return(token);
            break;
        
        case SEMICOLON: 
            cont_sim_lido++;
            printf("<;, >\n");
            token.nome_token = SEMICOLON;
            token.atributo = -1;
            estado = ESTADO_INICIAL;
            return(token);
            break;

        case ASSIGN: 
            cont_sim_lido++;
            printf("<=, >\n");
            token.nome_token = ASSIGN;
            token.atributo = -1;
            estado = ESTADO_INICIAL;
            return(token);
            break;
        
        case LEFT_PARENTHESIS: 
            cont_sim_lido++;
            printf("<(, >\n");
            token.nome_token = LEFT_PARENTHESIS;
            token.atributo = -1;
            estado = ESTADO_INICIAL;
            return(token);
            break;

        case RIGHT_PARENTHESIS: 
            cont_sim_lido++;
            printf("<), >\n");
            token.nome_token = RIGHT_PARENTHESIS;
            token.atributo = -1;
            estado = ESTADO_INICIAL;
            return(token);
            break;

        case LEFT_BRACKET:
            cont_sim_lido++;
            printf("<{, >\n");
            token.nome_token = LEFT_BRACKET;
            token.atributo = -1;
            estado = ESTADO_INICIAL;
            return(token);
            break; 

        case RIGHT_BRACKET:
            cont_sim_lido++;
            printf("<}, >\n");
            token.nome_token = RIGHT_BRACKET;
            token.atributo = -1;
            estado = ESTADO_INICIAL;
            return(token);
            break;

        case COMMA:
            cont_sim_lido++;
            printf("<,, >\n");
            token.nome_token = COMMA;
            token.atributo = -1;
            estado = ESTADO_INICIAL;
            return(token);
            break;

        case OP_SUM:
            cont_sim_lido++;
            printf("<+, >\n");
            token.nome_token = OP_SUM;
            token.atributo = -1;
            estado = ESTADO_INICIAL;
            return(token);
            break;

        case OP_SUB:
            cont_sim_lido++;
            c = code[cont_sim_lido];
            if(isdigit(c)){
              estado = NUMBER;
            }
            else if(c == '-'){
              estado = SMALL_COMMENTARY;
            }
            else{
              printf("<-, >\n");
              token.nome_token = OP_SUB;
              token.atributo = -1;
              estado = ESTADO_INICIAL;
              return(token);
            }
            break;

        case OP_MUL:
            cont_sim_lido++;
            printf("<*, >\n");
            token.nome_token = OP_MUL;
            token.atributo = -1;
            estado = ESTADO_INICIAL;
            return(token);
            break;

        case OP_DIV:
            cont_sim_lido++;
            printf("</, >\n");
            token.nome_token = OP_DIV;
            token.atributo = -1;
            estado = ESTADO_INICIAL;
            return(token);
            break;
      
      }

    }
}

int main() {
  Token token;
  code = readFile("programa.txt");
  do {
    token = proximo_token();
  } while (token.nome_token != EOF);
  free(code);
  free(tabela_simbolos);
}