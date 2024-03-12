#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <strings.h>
#include "scanner.h"
#include "testScanner.h"

int fsaTable[12][12] = {
        {1001,0,1,3,10,8,6,5,-1,-2,-3,-4 },
        {-5,-5,-5,-5,-5,-5,-5,-5,4,-5,-5,-5},
        {1002,1002,1002,1002,1002,1002,1002,1002,2,1002,1002,1002},
        {-5,-5,-5,-5,-5,-5,-5,-5,4,-5,-5,-5},
        {1003,1003,1003,1003,1003,1003,1003,1003,4,1003,1003,1003},
        {1004,1004,1004,1004,1004,1004,1004,1004,1004,1004,1004,1004},
        {1004,1004,1004,1004,1004,1004,1004,1004,1004,1004,1004,7},
        {1004,1004,1004,1004,1004,1004,1004,1004,1004,1004,1004,1004},
        {1004,1004 ,1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 9, 1004},
        {1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004},
        {1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 11, 1004, 1004},
        {1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004, 1004}
};
//Function for getting column number of corresponding lookahead
int charToColumn(int ch) {
if (ch == EOF){ return 0;}
    if (ch == ' ' || ch == '\t') {return 1;}
    if (isalpha(ch)) {return 2;}
    if (ch == '%') {return 3;}
    if (ch == ',') {return 4;}
    if (ch == '?') {return 5;}
    if (ch == '*') {return 6;}
    if (ch == '.' || ch == '!') {return 7;}
    if (isdigit(ch)) {return 8;}
    if (ch == ';') {return 9;}
    if (ch == '$') {return 10;}
    if (ch == '"') {return 11;}
    return -1;
}


struct token FADriver(int *lineNumber){
        int state = 0; // Start state
        int column;
        char lexeme[100] = {0}; // Buffer to build the lexeme
        int lexIndex = 0; // Index for lexeme building
        struct token newToken;
        char* strdup(const char*);

    while (1) {
        //Get column number of lookahead in FSA table
        column = charToColumn(lookahead);
        if(column < 0){
        printf("SCANNER ERROR COLUMN: Invalid character '%c' at line %d\n", lookahead, *lineNumber);
        exit(EXIT_FAILURE); // or handle the error as required
        }
        int nextState = fsaTable[state][column];

        //Check if nextState is an accepting state, error state, or requires more input
        if (nextState >= 1001 && nextState <= 1004) {
                if(lookahead != EOF){
                        lexeme[lexIndex++] = lookahead;
                        lexeme[lexIndex] = '\0'; //Null-terminate the lexeme string
                }
                if(nextState == 1001){newToken.id = EOF_tk;}
                if(nextState == 1002){newToken.id = T1_tk;}
                if(nextState == 1003){newToken.id = T2_tk;}
                if(nextState == 1004){newToken.id = T3_tk;}
                //if its EOF token make lexeme
                if(newToken.id == EOF_tk){
                        newToken.tokenInstance = strdup("\0");
                }else{
                        newToken.tokenInstance = strdup(lexeme);

                }
                newToken.lineNumber = *lineNumber;
                return newToken;
        } else if (nextState < 0) {
                //Error states
                 if(lookahead != EOF){
                        lexeme[lexIndex++] = lookahead;
                        lexeme[lexIndex] = '\0'; // Null-terminate the lexeme string
                }

                if (nextState == -1) {
                        newToken.id = ERROR_NUMBER_START;
                } else if (nextState == -2) {
                        newToken.id = ERROR_SEMICOLON_START;
                } else if (nextState == -3) {
                        newToken.id = ERROR_DOLLAR_START;
                } else if (nextState == -4) {
                        newToken.id = ERROR_QUOTE_START;
                } else if (nextState == -5) {
                        newToken.id = ERROR_MUST_BE_NUMBER;
                }
                newToken.tokenInstance = strdup(lexeme);
                newToken.lineNumber = *lineNumber;
                return newToken;

        } else {//Else a continuation for next state
                // Add character to lexeme and move to the next state
                lexeme[lexIndex++] = lookahead;
                state = nextState;
                lookahead = fgetc(inputFile); // Get next character
                if (lookahead == '\n') //Skip if new line(Caused issues moving to next token)
                {
                        (*lineNumber)++;
                        lookahead = fgetc(inputFile); // Get next character
                }
        }
    }

}
