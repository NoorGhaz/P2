#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "scanner.h"
#include "token.h"
#include "testScanner.h"

int lookahead;
void testScanner() {
        lookahead = fgetc(inputFile); // Get the first character from the file
        int lineNumber = 1;

        //While loop to call on Scanner
        while (lookahead != EOF) {
                if (lookahead == '#') { // Skip the entire comment line
                        while ((lookahead = fgetc(inputFile)) != '#' && lookahead != EOF){
                                if(lookahead == '\n'){
                                        lineNumber++;
                                }
                        }
                } else if (lookahead == '\n') { //If starts with new line, increase line number and get next char
                        lineNumber++;
                        lookahead = fgetc(inputFile);
                } else { //Valid character to pass to scanner
                  struct token nextToken = FADriver(&lineNumber);
                     if(nextToken.id >= EOF_tk && nextToken.id <= T3_tk){ //Using enum comparison when they were declared
                        printf("Token: %s, Lexeme:%s, Line: %d\n", tokenNames[nextToken.id], nextToken.tokenInstance, nextToken.lineNumber);
                        }else{
                        printf("%s, ERROR ON LEXEME:%s,Line: %d\n", tokenNames[nextToken.id], nextToken.tokenInstance, nextToken.lineNumber);
                        exit(EXIT_FAILURE);
                     }

                }

                lookahead = fgetc(inputFile); //Get the next character for the next iteration
                if (lookahead == '\n') {//Skip over new line
                        lookahead = fgetc(inputFile);
                }
        }
        //Handle EOF token outside of EOF
        if (lookahead == EOF) {
                struct token eofToken;
                eofToken.id = EOF_tk; // Assuming EOF_tk is the enum value for EOF
                eofToken.tokenInstance = NULL; // No lexeme associated with EOF
                eofToken.lineNumber = lineNumber;
                printf("Token: %s, Lexeme: %s, Line: %d\n", tokenNames[eofToken.id], eofToken.tokenInstance ? eofToken.tokenInstance : "EOF", eofToken.lineNumber);
        }
}
