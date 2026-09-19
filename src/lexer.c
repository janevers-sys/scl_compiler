#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexer.h"
#include <ctype.h>

#define MAX_TOKENS 1024

char *source;
int current;
int length;
int line;



struct wordstuff list_id[] = {
    {"if", T_IF},
    {"display", T_PRINT},
    {"else", T_ELSE},
    {"num", T_INT},
    {"(", T_LEFT_KLAMMER},
    {")", T_RIGHT_KLAMMER},
    {"=", T_IS},
    {".", T_DOT},
    {";", T_SEMICOLON},
    {"+", T_PLUS},
    {"-", T_MINUS},
    {"*", T_MAL},
    {"/", T_GETEILT},
    {"\"", T_STRING},    
    {"", T_IDENTIFIER},

};

void print_array(struct Tarray *tokens) {
    for(int i = 0; i < tokens->size; i++) {
        const struct lexer *t = &tokens->data[i];
        printf("type=%d line=%d column=%d lexeme: %s\n",t->type, t->line, t->column, t->lexeme);
    }
    return;
}

struct Tarray start_array(void) {
    struct Tarray array;
    array.size = 0;
    array.length = 5;
    array.data = malloc(array.length * sizeof(struct lexer));
    return array;
}

void push_array(struct Tarray *array, struct lexer token) {
    if(array->length == array->size) {
        void *warte = realloc(array->data, array->length * 2 * sizeof(struct lexer));
        if(warte == 0) {
            free(array->data);
            array->length = 0;
            array->size = 0;
            return;
        } 
        array->length *= 2;
        array->data = warte;
    }
        array->data[array->size] = token;
        array->size++;
        return;
    }


void free_array(struct Tarray *array) {
    for (size_t i = 0; i < array->size; i++) {
        free(array->data[i].lexeme);
    }
    array->length = 0;
    array->size = 0;
    free(array->data);
    return;
}

bool end() {
    return current >= length;
    
}

char advance() {
    char c = source[current];
    current++;
    return c;
}

char peek() {
    if(end()) {
        return '\0';
    }
    char c = source[current];
    return c;
}

bool match(char karak)   {
    if(end()) {
        return false;
    }
    if(source[current] == karak) {
        current++;
        return true;
    } else {
        return false;
    }
}


char *extract(int start, int end) {
    int word_length = end - start;
    char* text = malloc(word_length + 1);
    if(text == 0) {
        return 0;
    }

    for(int i = 0; i < word_length; i++) {
        text[i] = source[start + i];
    }
    text[word_length] = '\0';
    return text;
}


int lexer_start(char *filename) {
    FILE *fp = fopen(filename, "r");
        if (fp == 0) {
        perror("Could not open file\n");
        return 1;
    }
    fseek(fp, 0L, SEEK_END);
    size_t size = ftell(fp);
    rewind(fp);
    char *buffer = malloc(size + 1);

    if(buffer == NULL) {
        printf("error malloc\n");
        return 1;
    }

    size_t read = fread(buffer, sizeof(char), size, fp);
    if(read < size) {
        printf("error reading file\n");
        return 1;
    }
    buffer[read] = '\0';
    source = buffer;       
    length = read;         
    current = 0;
    struct Tarray tokens = start_array();
    struct lexer t;
    while(end() != true) {

        int start = current;
        char c = advance();
        t.type = 1;

        switch(c) {
            case '\t':
            case '\r':
            case ' ':
            break;

            case '\n':
            line++;
            break;

            case '=':
            t.type = T_IS;
            break;
            case '(':
            t.type = T_LEFT_KLAMMER;
            break;
            case ')':
            t.type = T_RIGHT_KLAMMER;
            break;
            case '.':
            t.type = T_DOT;
            break;
            case '-':
            t.type = T_MINUS;
            break;
            case '+':
            t.type = T_PLUS;
            break;
            case '*':
            t.type = T_MAL;
            break;
            case '/':
            t.type = T_GETEILT;
            break;
            case ';':
            t.type = T_SEMICOLON;
            break;
            case '\"':
            while(peek() != '\"' && !end()) {
                if(peek() == '\n') line++;
                advance();
            }
            if(end()) {
                printf("Error: Faulty string on line %d\n", line);
            } else {
                advance();
                t.type = T_STRING;
            }
            break;

            default:
            if(isalpha(c)) {
                while(isalnum(peek())) {
                    advance();
                }
                t.type = T_IDENTIFIER;
            } else if(isdigit(c)) {
                while(isdigit(peek())) {
                    advance();
                }
                t.type = T_INT;
            }
            break;


        }
         if(t.type != 1) {
            t.lexeme = extract(start, current);
            t.line = line;
            t.column = start;
            push_array(&tokens, t);
    }
    }
    fclose(fp);
    source = buffer;




    print_array(&tokens);

    free_array(&tokens);
    free(source);
    return 0;

}