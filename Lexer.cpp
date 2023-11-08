//
// Created by plusleft on 11/6/2023.
//

#include "Lexer.h"
#include "Reader.h"
#include "utils.h"
#include <iostream>

using namespace std;

Lexer::Lexer(string content) {
    this->content = content;
}

vector<Token> Lexer::lex() {
    // split the file into lines
    vector<string> lines = split(this->content, "\n");

    // lex each line
    vector<Token> tokens;
    for (int i = 0; i < lines.size(); i++) {
        vector<Token> line_tokens = lex_line(lines[i], i);
        // add eol token
        if (line_tokens.size() > 0)
            line_tokens.insert(line_tokens.end(), Token("EOL", i, lines[i].size() + 1, EOL));
        tokens.insert(tokens.end(), line_tokens.begin(), line_tokens.end());
    }

    return tokens;
}

// this should split up to make it more readable
vector<Token> Lexer::lex_line(string line_content, int line) {
    vector<Token> tokens;

    bool was_type = false;
    bool function_start = false;
    string token_value = "";
    TokenType state = WHITESPACE;

    for (int i = 0; i < line_content.size(); i++) {
        current_char = line_content[i];
        int column = i + 1;

        // comment ~
        if (current_char == '~') {
            if (state == WHITESPACE) {
                state = COMMENT;
            } else if (state == COMMENT) {
                state = WHITESPACE;
            }
        }

        // type word lemon mood once per line
        else if ((token_value == "wor" && current_char == 'd' || token_value == "lemo" && current_char == 'n' || token_value == "moo" && current_char == 'd' || token_value == "voi" && current_char == 'd') && !was_type) {
            if (state == WHITESPACE) {
                state = TYPE;
                was_type = true;
            } else if (state == TYPE) {
                continue;
            } else if (state == COMMENT) {
                continue;
            } else {
                error("While parsing type", "Unexpected character", line, column);
            }
        }

        // string literal
        else if (current_char == '"') {
            if (state == WHITESPACE || token_value == "->") {
                state = LITERAL;
                continue;
            } else if (state == LITERAL) {
                tokens.push_back(Token(token_value, line, column, state));
                token_value = "";
                state = WHITESPACE;
                continue;
            } else if (state == COMMENT) {
                continue;
            } else {
                error("While parsing string literal", "Unexpected character", line, column);
            }
        }

        // number (lemon)
        else if (isdigit(current_char) && was_type && state != LITERAL && state != KEYWORD) {
            if (state == WHITESPACE) {
                state = LITERAL;
                tokens.push_back(Token(token_value + current_char, line, column, state));
                token_value = "";
                state = WHITESPACE;
                continue;
            } else if (state == COMMENT) {
                continue;
            } else if (state == LITERAL) {
                // do nothing
            } else {
                error("While parsing number", "Unexpected character", line, column);
            }
        }

        // operator
        else if ((current_char == '+' || current_char == '-' || current_char == '*' || current_char == '/' || current_char == '%' || current_char == '=' || current_char == '!') && state != LITERAL && state != KEYWORD) {
            if (state == WHITESPACE) {
                state = OPERATOR;
            } else if (state == COMMENT) {
                continue;
            } else {
                tokens.push_back(Token(token_value, line, column, state));
                token_value = "";
                state = WHITESPACE;
                continue;
            }
        }

        // keywords if, (good, bad) moods, f-> to start function <-f to end function, ask, say, sheesh (shell commands), get (import)
        else if ((token_value == "i" && current_char == 'f'
        || token_value == "f-" && current_char == '>'
        || token_value == "<-" && current_char == 'f'
                ) && state != LITERAL && state != KEYWORD) {
            if (state == COMMENT) {
                continue;
            } else {
                if (token_value == "f-" && current_char == '>') {
                    function_start = true;
                } else if (token_value == "<-" && current_char == 'f') {
                    function_start = false;
                }
                state = KEYWORD;
                // append to tokens
                tokens.push_back(Token(token_value + current_char, line, column, state));
                token_value = "";
                state = WHITESPACE;
                continue;
            }
        }

        // identifier
        else if ((isalpha(current_char) || current_char == '_') && was_type && state != LITERAL && state != KEYWORD) {
            if (state == WHITESPACE) {
                state = IDENTIFIER;
            } else if (state == COMMENT) {
                continue;
            } else if (state == IDENTIFIER) {
                // do nothing
            } else {
                printf("Token value: %s\n", token_value.c_str());
                printf("State: %d\n", state);
                error("While parsing identifier", "Unexpected character", line, column);
            }
        }

        // function name and arsg f-> name : arg : arg
        else if (current_char == ':' && function_start) {
            if (state == WHITESPACE) {
                tokens.push_back(Token(token_value, line, column, IDENTIFIER));
                token_value = "";
                state = WHITESPACE;
                continue;
            } else if (state == COMMENT) {
                continue;
            } else {
                error("While parsing function name", "Unexpected character", line, column);
            }
        }

        // whitespace
        else if ((current_char == ' ' || current_char == '\t' || current_char == '\n') && state != LITERAL) {
            if (state == WHITESPACE) {
                // todo
                continue;
            } else if (state == COMMENT) {
                continue;
            } else {
                tokens.push_back(Token(token_value, line, column, state));
                token_value = "";
                state = WHITESPACE;
                continue;
            }
        }

        // delimiter only one in our case which is : which stands for =
        else if (current_char == ':') {
            if (state == COMMENT) {
                continue;
            } else {
                token_value = current_char;
                tokens.push_back(Token(token_value, line, column, DELIMITER));
                token_value = "";
                state = WHITESPACE;
                continue;
            }
        }

        // helper for string literal
        if ((current_char == ' ' || current_char == '\t') && state != LITERAL) {
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