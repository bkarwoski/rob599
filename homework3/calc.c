#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>

void skip_whitespace(char **str) {
    while (isspace(*str[0])) {
        (*str)++;
    }
}

char peek(char **str) {
    skip_whitespace(str);
    return *str[0];
}

char parse_token(char **str) {
    skip_whitespace(str);
    char token = *str[0];
    if (!token) {
        fprintf(stderr, "parsing error. unexpected end of string\n");
        exit(1);
    }
    (*str)++;
    return token;
}

double add_expression(char **input);

double num_literal(char **input) {
    double val = 0;
    double sign = 1;
    bool hasDigits = false;
    if (peek(input) == '+') {
        parse_token(input);
    } else if (peek(input) == '-') {
        sign = -1;
        parse_token(input);
    }
    if(peek(input) >= '0' && peek(input) <= '9') {
        hasDigits = true;
        while(peek(input) >= '0' && peek(input) <= '9') {
            val = val * 10 + (parse_token(input) - '0');
        }
    } else if (peek(input) != '.') {
        printf("error, expected a num or '.',got a %c\n",
               parse_token(input));
        exit(1);
    }
    if (peek(input) == '.') {
        parse_token(input);
        double decVal = 0;
        double count = 0;
        double decIndex = 1;
        while(peek(input) >= '0' && peek(input) <= '9') {
            hasDigits = true;
            decIndex /= 10;
            val += decIndex * (parse_token(input) - '0');
        }
    }
    if (!hasDigits) {
        printf("error, no digits parsed\n");
        exit(1);
    }
    return val * sign;
}

double paren_expression(char **input) {
    double val = 0;
    if (peek(input) == '(') {
        parse_token(input);
        val = add_expression(input);
        if (peek(input) != ')') {
            printf("error, expecting closing parenthesis\n");
            exit(1);
        } else {
            parse_token(input);
        }
    } else {
        val = num_literal(input);
    }
return val;
}

double mult_expression(char **input) {
    double val = paren_expression(input);
    while (peek(input) == '*' || peek(input) == '/') {
        if (peek(input) == '*') {
            parse_token(input);
            val *= paren_expression(input);
        } else if (peek(input) == '/') {
            parse_token(input);
            val /= paren_expression(input);
        }
    }
    return val;  
}

double add_expression(char **input) {
    double val = mult_expression(input);
    while (peek(input) == '+' || peek(input) == '-') {
        if (peek(input) == '+') {
            parse_token(input);
            val += mult_expression(input);
        } else if (peek(input) == '-') {
            parse_token(input);
            val -= mult_expression(input);
        }
    }
    return val;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("usage: %s <statement>\n", argv[0]);
        return 1;
    }
    printf("%lf\n", add_expression(&argv[1]));
    //printf("parsed value = %lf\n", num_literal(&argv[1]));
    return 0;
}
