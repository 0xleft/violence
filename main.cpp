#include <iostream>
#include "Lexer.h"
#include "Reader.h"
#include "Parser.h"
#include <signal.h>
#include "InlineCHandler.h"
#include <cstring>

int handle_interactive_mode() {
    // read from stdin
    string line;
    Parser parser;

    printf(">>> ");
    while (getline(cin, line)) {
        Lexer lexer(line);
        vector<Token> tokens = lexer.lex();

        Variable output = parser.parse(tokens);

        if (output.get_value() != "") {
            printf("%s\n", output.get_value().c_str());
        }

        printf(">>> ");
    }

    return 0;
}

int handle_file_mode(char *filename) {
    Reader reader(filename);
    string content = reader.get_content();
    reader.close();

    if (content == "") {
        printf("Error: File not found\n");
        return 1;
    }

    Lexer lexer(content);
    vector<Token> tokens = lexer.lex();

    Parser parser;

    // todo
    Variable output = parser.parse(tokens);

    return 0;
}

void handle_sigint(int sig) {
    printf("\nCtrl-C pressed, exiting...\n");
    exit(0);
}

int main(int argc, char *argv[]) {

    signal(SIGINT, handle_sigint);

    // if argv[2] is --clean
    if (argc == 3 && strcmp(argv[2], "--clean") == 0) {
        printf("Cleaning...\n");
        InlineCHandler::clean();
    }

    // and not equal to --
    if (argv[1] != NULL && strcmp(argv[1], "--") != 0) {
        handle_file_mode(argv[1]);
    } else {
        handle_interactive_mode();
    }

    return 0;
}