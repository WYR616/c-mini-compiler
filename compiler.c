#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ======================
// 1. 词法分析：Token 类型
// ======================
typedef enum {
    TOKEN_NUMBER,  // 数字
    TOKEN_PLUS,    // +
    TOKEN_MINUS,   // -
    TOKEN_MUL,     // *
    TOKEN_DIV,     // /
    TOKEN_LPAREN,  // (
    TOKEN_RPAREN,  // )
    TOKEN_EOF      // 结束
} TokenType;

typedef struct {
    TokenType type;
    int value;  // 数字的值
} Token;

// 当前 token
Token currentToken;
// 源代码字符串
const char* code;
int pos = 0;

// 跳过空格
void skipWhitespace() {
    while (code[pos] && isspace(code[pos])) pos++;
}

// 获取下一个 Token
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

    // 运算符
    switch (c) {
        case '+': currentToken.type = TOKEN_PLUS; break;
        case '-': currentToken.type = TOKEN_MINUS; break;
        case '*': currentToken.type = TOKEN_MUL; break;
        case '/': currentToken.type = TOKEN_DIV; break;
        case '(': currentToken.type = TOKEN_LPAREN; break;
        case ')': currentToken.type = TOKEN_RPAREN; break;
        default:
            printf("错误：未知字符 %c\n", c);
            exit(1);
    }
    pos++;
}

// ======================
// 2. 语法分析 + 计算
// ======================
int parseFactor();
int parseTerm();
int parseExpr();

// 表达式：加减
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

// 项：乘除
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

// 因子：数字 / 括号
int parseFactor() {
    int val;
    if (currentToken.type == TOKEN_LPAREN) {
        nextToken();
        val = parseExpr();
        if (currentToken.type != TOKEN_RPAREN) {
            printf("错误：缺少右括号\n");
            exit(1);
        }
        nextToken();
        return val;
    } else if (currentToken.type == TOKEN_NUMBER) {
        val = currentToken.value;
        nextToken();
        return val;
    } else {
        printf("语法错误\n");
        exit(1);
    }
}

// 编译并运行
int compile(const char* sourceCode) {
    code = sourceCode;
    pos = 0;
    nextToken();
    return parseExpr();
}

// ======================
// 主函数：测试编译器
// ======================
int main() {
    printf("=== C 语言迷你编译器 ===\n\n");

    const char* test1 = "1 + 2 * 3";
    printf("表达式：%s\n结果：%d\n\n", test1, compile(test1));

    const char* test2 = "(10 - 4) / 2";
    printf("表达式：%s\n结果：%d\n\n", test2, compile(test2));

    const char* test3 = "5 * (2 + 8)";
    printf("表达式：%s\n结果：%d\n\n", test3, compile(test3));

    return 0;
}