#include <iostream>
#include "Lexer.h"
#include "Reader.h"
#include "Parser.h"
#include "InlineCHandler.h"

void a() {
    InlineCHandler handler;
    handler.add_function("char *b() { return \"Hello World\"; }");
    handler.compile("c");
    vector<string> args = vector<string>();
    string output = handler.run("c", "b", args);
    printf("%s\n", output.c_str());
}

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

int main(int argc, char *argv[]) {

    a();

    if (argv[1] != NULL) {
        handle_file_mode(argv[1]);
    } else {
        handle_interactive_mode();
    }

    return 0;
}