#ifndef LEXER_H
#define LEXER_H
#include <stddef.h> 

enum Ttype {
    T_INT, 
    T_DOT,
    T_IDENTIFIER,
    T_INTIGER,
    T_ASSIGN,
    T_PLUS,
    T_PRINT,
    T_IF,
    T_ELSE,
    T_STRING,
    T_RIGHT_KLAMMER,
    T_LEFT_KLAMMER,
    T_SEMICOLON,
    T_IS,
    T_MINUS,
    T_MAL,
    T_GETEILT,
    T_VAR,
    T_ERROR
};


struct lexer {
    enum Ttype type;
    char *lexeme;
    int line;
    int column;
};

struct Tarray {
    size_t size;
    size_t length;
    struct lexer *data;
};

int lexer_start(char *Filename);
struct wordstuff {
    char *word;
    enum Ttype type;
};

#endif