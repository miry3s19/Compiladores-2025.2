#ifndef TOKENS_H
#define TOKENS_H

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

#endif