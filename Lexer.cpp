//
// Created by plusleft on 11/6/2023.
//

#include "Lexer.h"
#include "Reader.h"
#include "utils.h"
#include <iostream>

using namespace std;

Lexer::Lexer(string filename) {
    this->filename = filename;
}

vector<Token> Lexer::lex() {
    // read the file
    Reader reader(filename);
    string content = reader.get_content();
    reader.close();

    // split the file into lines
    vector<string> lines = split(content, "\n");

    // lex each line
    vector<Token> tokens;
    for (int i = 0; i < lines.size(); i++) {
        vector<Token> line_tokens = lex_line(lines[i], i);
        tokens.insert(tokens.end(), line_tokens.begin(), line_tokens.end());
    }

    printf("Lexing complete\n");
    print_tokens(tokens);

    return tokens;
}

void Lexer::print_tokens(vector<Token> tokens) {
    for (int i = 0; i < tokens.size(); i++) {
        cout << tokens[i].to_string() << endl;
    }
}

vector<Token> Lexer::lex_line(string line_content, int line) {
    vector<Token> tokens;

    bool is_string = false;
    bool is_comment = false;
    bool is_type = true;
    bool is_identifier = false;
    bool is_operator = false;
    string token_value = "";

    for (int i = 0; i < line_content.size(); i++) {
        current_char = line_content[i];

        if (current_char == '~') {
            is_comment = !is_comment;
        }

        if (is_comment) {
            continue;
        }

        if (current_char == '"') {
            if (is_string) {
                tokens.push_back(Token(token_value, line, i, LITERAL));
                token_value = "";
            }
            is_string = !is_string;
            continue;
        }

        if (is_string) {
            if (i == line_content.size() - 1) {
                error("", "Unterminated string", line, i);
            }
            token_value += current_char;
            continue;
        }

        if (current_char == ' ' || current_char == '\t') {
            if (token_value != "") {
                if (is_type) {
                    tokens.push_back(Token(token_value, line, i, TYPE));
                    is_type = false;
                    is_identifier = true;
                } else if (is_identifier) {
                    tokens.push_back(Token(token_value, line, i, IDENTIFIER));
                    is_identifier = false;
                }
                token_value = "";
            }
            continue;
        }

        if (current_char == ':') {
            if (is_type || is_identifier) {
                error("", "Unexpected ':'", line, i);
            }
            tokens.push_back(Token(":", line, i, OPERATOR));
            is_operator = true;
            continue;
        }

        if (is_operator && i == line_content.size() - 1) {
            error("", "Unexpected end of line", line, i);
        }

        // operator logic

        token_value += current_char;
    }

    return tokens;
}

Lexer::~Lexer() = default;

void Lexer::error(string trace, string message, int line, int column) {
    printf("%s", trace.c_str());
    printf("ERROR: %s at line %d, column %d\n", message.c_str(), line, column);
    exit(1);
}