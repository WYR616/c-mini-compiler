#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    TOKEN_NUMBER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MUL,
    TOKEN_DIV,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    int value;
} Token;

Token currentToken;
const char* code;
int pos = 0;

void skipWhitespace() {
    while (code[pos] && isspace(code[pos])) pos++;
}

void nextToken() {
    skipWhitespace();
    char c = code[pos];

    if (c == '\0') {
        currentToken.type = TOKEN_EOF;
        return;
    }

    if (isdigit(c)) {
        int num = 0;
        while (isdigit(code[pos])) {
            num = num * 10 + (code[pos] - '0');
            pos++;
        }
        currentToken.type = TOKEN_NUMBER;
        currentToken.value = num;
        return;
    }

    switch (c) {
        case '+': currentToken.type = TOKEN_PLUS; break;
        case '-': currentToken.type = TOKEN_MINUS; break;
        case '*': currentToken.type = TOKEN_MUL; break;
        case '/': currentToken.type = TOKEN_DIV; break;
        case '(': currentToken.type = TOKEN_LPAREN; break;
        case ')': currentToken.type = TOKEN_RPAREN; break;
        default:
            printf("Error: unknown character %c\n", c);
            exit(1);
    }
    pos++;
}

int parseFactor();
int parseTerm();
int parseExpr();

int parseExpr() {
    int left = parseTerm();
    while (currentToken.type == TOKEN_PLUS || currentToken.type == TOKEN_MINUS) {
        TokenType op = currentToken.type;
        nextToken();
        int right = parseTerm();
        if (op == TOKEN_PLUS) left += right;
        else left -= right;
    }
    return left;
}

int parseTerm() {
    int left = parseFactor();
    while (currentToken.type == TOKEN_MUL || currentToken.type == TOKEN_DIV) {
        TokenType op = currentToken.type;
        nextToken();
        int right = parseFactor();
        if (op == TOKEN_MUL) left *= right;
        else left /= right;
    }
    return left;
}

int parseFactor() {
    int val;
    if (currentToken.type == TOKEN_LPAREN) {
        nextToken();
        val = parseExpr();
        if (currentToken.type != TOKEN_RPAREN) {
            printf("Error: missing closing parenthesis\n");
            exit(1);
        }
        nextToken();
        return val;
    } else if (currentToken.type == TOKEN_NUMBER) {
        val = currentToken.value;
        nextToken();
        return val;
    } else {
        printf("Syntax error\n");
        exit(1);
    }
}

int compile(const char* sourceCode) {
    code = sourceCode;
    pos = 0;
    nextToken();
    return parseExpr();
}

int main() {
    printf("=== C Mini Compiler ===\n\n");

    const char* test1 = "1 + 2 * 3";
    printf("Expression: %s\nResult: %d\n\n", test1, compile(test1));

    const char* test2 = "(10 - 4) / 2";
    printf("Expression: %s\nResult: %d\n\n", test2, compile(test2));

    const char* test3 = "5 * (2 + 8)";
    printf("Expression: %s\nResult: %d\n\n", test3, compile(test3));

    return 0;
}