#include <iostream>
#include "Lexer.h"
#include "Reader.h"

int handle_interactive_mode() {
    // read from stdin
    string line;
    printf(">>> ");
    while (getline(cin, line)) {
        Lexer lexer(line);
        vector<Token> tokens = lexer.lex();

        // todo
        lexer.print_tokens(tokens);

        printf(">>> ");
    }

    return 0;
}

int handle_file_mode(char *filename) {
    Reader reader(filename);
    string content = reader.get_content();
    reader.close();

    Lexer lexer(content);
    vector<Token> tokens = lexer.lex();

    // todo
    lexer.print_tokens(tokens);

    return 0;
}

int main(int argc, char *argv[]) {

    if (argv[1] != NULL) {
        handle_file_mode(argv[1]);
    } else {
        handle_interactive_mode();
    }

    return 0;
}