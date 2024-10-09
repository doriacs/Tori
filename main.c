#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SOURCE_FILE_ENDING      ".tori"

#define DEFAULT_STRING_SIZE     64
#define STRING_SIZE_ADD_MEM     64

#define DEFAULT_TOKEN_NO        1000 //tmp
#define TOKEN_NO_ADD_MEM        32

// single char tokens
#define DOT             0   // .
#define COMMA           1   // ,
#define SEMICOLON       2   // ;
#define OPEN_PAREN      3   // (
#define CLOSE_PAREN     4   // )
#define OPEN_SQ_PAREN   5   // [
#define CLOSED_SQ_PAREN 6   // ]
#define OPEN_BRACE      7   // {
#define CLOSED_BRACE    8   // }
#define PLUS            9   // +
#define MINUS           10  // -
#define STAR            11  // *
#define EQUAL           12  // =
#define NOT             13  // !
#define DIV             14  // /
#define LESS_THAN       15  // <
#define GREATER_THAN    16  // >

struct token {
    int     type;
    char*   lit_val; //var names, func names etc.
    int     line; // line and col at which the token is found (for dbg)
    int     col;
};

struct token_list {
    struct token**  tokens;
    int             size;
    int             capacity;
};

/*
    func main(arg1, arg2, ...) -> void
    {

        sint32 a;
        a = 2;
        
        // sint8, sint16, sint32, sint64
        // uint8, ..., uint64
        // float
        // double
        // struct
    }
*/

int usage_error(int argc, char** argv) {

    FILE* f;

    if (argc == 1 || argc > 2) {
        fprintf(stderr, "Usage: ./tori <filename>.tori\n");
        return 1;
    }

    char* tmp_ending = strstr(argv[1], SOURCE_FILE_ENDING);
    if (!tmp_ending) {
        fprintf(stderr, "Not a valid file. (It must have the \".tori\" file ending.)\n");
        return 1;
    }

    if (strcmp(tmp_ending, SOURCE_FILE_ENDING)) {
        fprintf(stderr, "Not a valid file. (It must have the \".tori\" file ending.)\n");
        return 1;
    } else {
        f = fopen(argv[1], "r");
        if (f == NULL) {
            fprintf(stderr, "File does not exist.\n");
            return 1;
        } else {
            return 0;
        }
    }
}

char* read_input(char* input_file) {

    char*   raw_input;
    FILE*   f_input_file;
    int     curr_char_num, max_char_num, i;

    if (!input_file)
        return NULL;

    f_input_file = fopen(input_file, "r");
    if (f_input_file == NULL)
        return NULL;

    curr_char_num   = 0;
    max_char_num    = DEFAULT_STRING_SIZE;

    raw_input = calloc(max_char_num, sizeof(char));
    if (raw_input == NULL)
        return NULL;

    while(!feof(f_input_file)) {
        raw_input[curr_char_num] = fgetc(f_input_file);
        
        curr_char_num++;

        if (curr_char_num >= max_char_num) {
            max_char_num += STRING_SIZE_ADD_MEM;

            raw_input = realloc(raw_input, max_char_num);

            if (raw_input == NULL)
                return NULL;  
        }
    }

    fclose(f_input_file);

    raw_input[curr_char_num - 1] = '\0';

    return raw_input;
}

struct token_list* create_token_list() {
    struct token_list* tkn_l = calloc(1, sizeof(struct token_list));
    if (!tkn_l)
        return NULL;

    tkn_l->tokens = calloc(DEFAULT_TOKEN_NO, sizeof(struct token*));
    if (!tkn_l->tokens) {
        free(tkn_l);
        return NULL;
    }

    tkn_l->size     = 0;
    tkn_l->capacity = DEFAULT_TOKEN_NO; 

    return tkn_l;    
}

int add_token(struct token_list* tkn_l, int token_type, char* literal_val,
              int line, int col) {
    if (tkn_l->size == tkn_l->capacity - 1) {
        // realloc TODO
    }

    int idx = tkn_l->size;

    tkn_l->tokens[idx] = calloc(1, sizeof(struct token));
    if (!tkn_l->tokens[idx]) {
        for (int i = 0; i < idx; ++i) {
            free(tkn_l->tokens[i]); // TODO for literals free
        }

        free(tkn_l->tokens);
        free(tkn_l);

        return -1;
    }

    tkn_l->tokens[idx]->type    = token_type;
    tkn_l->tokens[idx]->line    = line;
    tkn_l->tokens[idx]->col     = col;

    // TODO check for literals
    tkn_l->tokens[idx]->lit_val = literal_val;

    tkn_l->size++;

    return 0;
}

struct token_list* tokenize_input(char* input) {
    int     curr_line, curr_col;
    char    c;
    size_t  len;

    struct token_list* tkn_l;

    curr_line   = 1;
    curr_col    = 1;

    len         = strlen(input);

    tkn_l       = create_token_list();
    if (!tkn_l)
        return NULL;

    for (size_t i = 0; i < len; ++i) {
        c = input[i];

        switch (c) {
            case '.':
                add_token(tkn_l, DOT, NULL, curr_line, curr_col);
                break;
            case '\n':
                curr_line++;
                curr_col = 0;
                break;
            default:
                break;
        }

        curr_col++;
    }



    return tkn_l;
}

int main (int argc, char** argv) {
    
    if (usage_error(argc, argv))
        return -1;

    struct token_list* tkn_l;

    char* ri = read_input(argv[1]);
    size_t len = strlen(ri);

    for (size_t i = 0; i < len; ++i) {
        printf("%d ", (int)ri[i]);
    }

    printf("%s\n", read_input(argv[1]));

    //tkn_l = create_token_list();
    //add_token(tkn_l, COMMA, NULL, 12, 2);

    tkn_l = tokenize_input(ri);

    for (int i = 0; i < tkn_l->size; ++i) {
        printf("%d %d %d\n", tkn_l->tokens[i]->type, tkn_l->tokens[i]->line,
               tkn_l->tokens[i]->col);
    }

    return 0;
}