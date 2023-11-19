//
// Created by plusleft on 11/6/2023.
//

#include "Lexer.h"
#include "Reader.h"
#include "utils.h"
#include <iostream>
#include "InlineCHandler.h"

using namespace std;

Lexer::Lexer(string content) {
    this->content = content;
}

vector<Token> Lexer::lex() {
    // split the file into lines
    vector<string> lines = split(this->content, "\n");

    bool is_inline_c = false;

    // lex each line
    vector<Token> tokens;
    string inline_c = "";
    for (int i = 0; i < lines.size(); i++) {
        if (lines[i].size() == 0) {
            continue;
        }
        if (lines[i][lines[i].size() - 1] == '\r') {
            lines[i].erase(lines[i].size() - 1);
        }

        if (lines[i].substr(0, 3) == "[c]") {
            is_inline_c = true;
            continue;
        } else if (lines[i].substr(0, 4) == "[/c]") {
            is_inline_c = false;
            tokens.push_back(Token(inline_c, i, 0, INLINE_C));
            continue;
        }

        if (is_inline_c) {
            inline_c += lines[i] + "\n";
            continue;
        }

        // last letter
        if (lines[i][lines[i].size() - 1] != '~') {
            // dont really like this it should automatically add it
            lines[i] += " ~";
        }

        // printf("Line: %s\n", lines[i].c_str());
        vector<Token> line_tokens = lex_line(lines[i], i);
        // print last letter
        // add eol token
        if (line_tokens.size() > 0)
            line_tokens.insert(line_tokens.end(), Token("EOL", i, lines[i].size() + 1, EOL));
        tokens.insert(tokens.end(), line_tokens.begin(), line_tokens.end());
    }

    return tokens;
}

// this should be made so it splits on space and then lex each word
// but maybe later
vector<Token> Lexer::lex_line(string line_content, int line) {
    vector<Token> tokens;

    string token_value = "";
    char next_char;
    TokenType state = WHITESPACE;

    for (int i = 0; i < line_content.size(); i++) {
        current_char = line_content[i];
        next_char = line_content[i + 1];

        int column = i + 1;

        // comment ~
        if (current_char == '~') {
            // add prev token
            if (token_value != "") {
                tokens.push_back(Token(token_value, line, column, state));
                token_value = "";
            }

            state = COMMENT;
            continue;
        }

        // type word lemon mood once per line
        else if (token_value == "wor" && current_char == 'd'
        || token_value == "lemo" && current_char == 'n'
        || token_value == "moo" && current_char == 'd'
        || token_value == "voi" && current_char == 'd') {
            tokens.push_back(Token(token_value + current_char, line, column, TYPE));
            token_value = "";
            state = WHITESPACE;
            continue;
        }

        // keywords if, f-> to start function <-f to end function
        else if ((token_value == "i" && current_char == 'f'
                  || token_value == "f-" && current_char == '>'
                  || token_value == "f" && current_char == '-'
                  || token_value == "<-" && current_char == 'f'
                 ) && state != LITERAL_STRING) {

            state = KEYWORD;

            if (next_char == '>') {
                token_value += current_char;
                continue;
            }

            tokens.push_back(Token(token_value + current_char, line, column, KEYWORD));
            token_value = "";
            state = WHITESPACE;
            continue;
        }

        // delimiter only one in our case which is : which stands for =
        else if ((current_char == ':'  || current_char == ',') && state != LITERAL_STRING) {
            // add prev token
            if (token_value != "") {
                tokens.push_back(Token(token_value, line, column, state));
                token_value = "";
            }

            token_value = current_char;
            tokens.push_back(Token(token_value, line, column, DELIMITER));
            token_value = "";
            state = WHITESPACE;
            continue;
        }

        // indexer start and end
        else if ((current_char == '['  || current_char == ']') && state != LITERAL_STRING) {
            // add prev token
            if (token_value != "") {
                tokens.push_back(Token(token_value, line, column, state));
                token_value = "";
            }

            token_value = current_char;
            if (current_char == '[') {
                tokens.push_back(Token("[", line, column, INDEXER_START));
            } else if (current_char == ']') {
                tokens.push_back(Token("]", line, column, INDEXER_END));
            }
            token_value = "";
            state = WHITESPACE;
            continue;
        }

        // string literal
        else if (current_char == '"') {
            if (token_value != "" && state != LITERAL_STRING) {
                tokens.push_back(Token(token_value, line, column, state));
                token_value = "";
                state = WHITESPACE;
            }

            if (state == WHITESPACE || token_value == "->") {
                state = LITERAL_STRING;
                continue;
            } else if (state == LITERAL_STRING) {
                tokens.push_back(Token(token_value, line, column, state));
                token_value = "";
                state = WHITESPACE;
                continue;
            } else {
                error("While parsing string literal", "Unexpected character", line, column);
            }
        }

        // number (lemon)
        else if (isdigit(current_char) && state != LITERAL_STRING && state != KEYWORD || current_char == '.' && state != LITERAL_STRING && state != KEYWORD) {
            if (token_value != "" && state != LITERAL_NUMBER) {
                tokens.push_back(Token(token_value, line, column, state));
                token_value = "";
                state = WHITESPACE;
            }

            if (state == WHITESPACE) {
                state = LITERAL_NUMBER;
                // tokens.push_back(Token(token_value + current_char, line, column, state));
                // token_value = "";
                // state = WHITESPACE;
                token_value += current_char;
                continue;
            } else if (state == COMMENT) {
                continue;
            } else if (state == LITERAL_NUMBER) {
                // do nothing
            } else {
                error("While parsing number", "Unexpected character", line, column);
            }
        }

        // operator
        else if ((current_char == '+'
        || current_char == '-'
        || current_char == '*'
        || current_char == '/'
        || current_char == '%'
        || current_char == '='
        || current_char == '!'
        || current_char == '>'
        || current_char == '<'
        || current_char == '^'
        || current_char == '&'
        || current_char == '|'
        ) && state != LITERAL_STRING && state != KEYWORD) {
            if (state == KEYWORD) {
                token_value += current_char;
                continue;
            }

            if (token_value != "" && state != OPERATOR) {
                tokens.push_back(Token(token_value, line, column, state));
                token_value = "";
                state = WHITESPACE;
            }

            state = OPERATOR;
            token_value += current_char;
            continue;
        }

        // if ( or ) set function start and function end
        else if ((current_char == '(' || current_char == ')') && state != LITERAL_STRING && state != KEYWORD) {
            if (token_value != "") {
                tokens.push_back(Token(token_value, line, column, state));
                token_value = "";
                state = WHITESPACE;
            }

            if (current_char == '(') {
                tokens.push_back(Token("(", line, column, FUNCTION_START));
                token_value = "";
                state = WHITESPACE;
            } else if (current_char == ')') {
                tokens.push_back(Token(")", line, column, FUNCTION_END));
                token_value = "";
                state = WHITESPACE;
            }

            continue;
        }

        // identifier
        else if ((isalpha(current_char) || current_char == '_') && state != LITERAL_STRING && state != KEYWORD) {

            if (token_value != "" && state != IDENTIFIER) {
                tokens.push_back(Token(token_value, line, column, state));
                token_value = "";
                state = WHITESPACE;
            }

            if (state == WHITESPACE) {
                state = IDENTIFIER;
            } else if (state == COMMENT) {
                continue;
            } else if (state == IDENTIFIER) {
                // do nothing
            } else {
                error("While parsing identifier", "Unexpected character", line, column);
            }
        }

        // dont add spaces unless its a string literal
        if ((current_char == ' ' || current_char == '\t') && state != LITERAL_STRING) {
            continue;
        }

        token_value += current_char;
    }

    return tokens;
}

Lexer::~Lexer() = default;

void Lexer::error(string trace, string message, int line, int column) {
    printf("%s\n", trace.c_str());
    printf("ERROR: %s at line %d, column %d\n", message.c_str(), line, column);
    exit(1);
}