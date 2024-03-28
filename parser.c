#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "scanner.h"
#include "token.h"
#include "parser.h"

/*

S -> C D
A -> F K
K -> F ?$ | .
B -> . t2 A !
C -> t2 *
D -> L
L -> empty | H ? L
E -> , A A H | ,; F H
F -> t1 | t2
G -> B | C | J
H -> E ? | G . | empty
J -> * ” A .

*/

/* FIRST SETS
S: t2
A: t1 t2
B: .
C: t2
D: , ,; . t2 *" epsilon ?
E: , | ,;
F: t1 | t2
G: . | t2 | *"
H: , ,; | . t2 *" | epsilon
J: * "
K: t1 t2 | .
L: , ,; . t2 * " epsilon ? | epsilon
*/

int lookahead;
struct token tokenLookahead;
int lineNumber = 1;

void testScanner() {
        lookahead = fgetc(inputFile); // Get the first character from the file

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
                tokenLookahead = FADriver(&lineNumber);
                if(tokenLookahead.id >= EOF_tk && tokenLookahead.id <= T3_tk){ //Using enum comparison when they were declared
                printf("Token: %s, Lexeme:%s, Line: %d\n", tokenNames[tokenLookahead.id], tokenLookahead.tokenInstance, tokenLookahead.lineNumber);
                  }else{
                        printf("%s, ERROR ON LEXEME:%s,Line: %d\n", tokenNames[tokenLookahead.id], tokenLookahead.tokenInstance, tokenLookahead.lineNumber);
                        exit(EXIT_FAILURE);
                     }

                }

        // lookahead = fgetc(inputFile); //Get the next character for the next iteration
        // if (lookahead == '\n') {//Skip over new line
        //         lookahead = fgetc(inputFile);
        // }

        S();
        if(tokenLookahead.id == EOF_tk){printf("Success\n");}else{printf("Failure\n");}
        //Handle EOF token outside of EOF
        // if (lookahead == EOF) {
        //         struct token eofToken;
        //         eofToken.id = EOF_tk;
        //         eofToken.tokenInstance = NULL; // No lexeme associated with EOF
        //         eofToken.lineNumber = lineNumber;
        //         printf("Token: %s, Lexeme: %s, Line: %d\n", tokenNames[eofToken.id], eofToken.tokenInstance ? eofToken.tokenInstance : "EOF", eofToken.lineNumber);
        // }
}

void S() {
    C();
    D();
}

void A() {
    // Implement logic based on the production rule A -> F K
    F();
    K();
}

void B() {
    // Implement logic based on the production rule B -> . t2 A !
    if(tokenLookahead.id == T3_tk && tokenLookahead.tokenInstance[0] == '.'){
        // Implement into tree when made
        tokenLookahead = FADriver(&lineNumber); //consume
        if(tokenLookahead.id == T2_tk){
            //process in tree when implemented
            tokenLookahead = FADriver(&lineNumber); //consume 't2'
            A();
            if(tokenLookahead.id == T3_tk && tokenLookahead.tokenInstance[0] == '!'){
            //process in tree when made
            tokenLookahead = FADriver(&lineNumber); //consume '!'
                return;
            }else{
                printf("ERROR IN PRODUCTION B: Expected '!'\n");
            }
        }else{
            printf("ERROR IN PRODUCTION B: Expected T2\n");
        }
    }
}

void C() {   // C -> t2 *

    if (tokenLookahead.id == T2_tk) {
        // Match found, consume token for 't2'
        tokenLookahead = FADriver(&lineNumber); // Get next token
        
        if (tokenLookahead.id == T3_tk && strcmp(tokenLookahead.tokenInstance, "*") == 0) {
        //move char lookahead furhter
        //   lookahead = fgetc(inputFile); //Get the next character for the next iteration
        //    if (lookahead == '\n') {//Skip over new line
        //         lookahead = fgetc(inputFile);
        //    }
            // Match found for '*', consume and proceed
            tokenLookahead = FADriver(&lineNumber); // Prepare next token for subsequent parsing
        } else {
            printf("Syntax error: Expected '*' after 't2', found %s on line %d\n", tokenLookahead.tokenInstance, tokenLookahead.lineNumber);
            exit(EXIT_FAILURE);
        }
    } else {
        printf("Syntax error: Expected 't2', found %s on line %d\n", tokenLookahead.tokenInstance, tokenLookahead.lineNumber);
        exit(EXIT_FAILURE);
    }
}

void D() {
    // Implement logic based on the production rule D -> L
    L();
    // Note: Handling recursion and empty production can be tricky; you might need to check for lookahead tokens.
}

void L() {
    // Check if the lookahead token matches any in the FIRST set of H
    if ((tokenLookahead.id == T3_tk && (tokenLookahead.tokenInstance[0] == ',' ||
                                        tokenLookahead.tokenInstance[1] == ';' ||
                                        tokenLookahead.tokenInstance[0] == '.')) ||
        (tokenLookahead.id == T2_tk) || // First set can be t2
        (tokenLookahead.id == T3_tk && (tokenLookahead.tokenInstance[0] == "*" && tokenLookahead.tokenInstance[1] == '"'))) {
        // The current token is in the FIRST set of H, go to H
        H();

        if (tokenLookahead.id == T3_tk && tokenLookahead.tokenInstance[0] == '?') {
            // If the current token is '?', consume it and recursively call L
            tokenLookahead = FADriver(&lineNumber); // Get next token
            L(); // Recursive call to handle "H ? L"
        } else{
        // If '?' is not immediately found after H, it's not an error here because L could simply be H without the recursion part.
        printf("ERROR, EXPECTED ? AFTER L\n");
        exit(EXIT_FAILURE);
        }
    }else{
    // If the token doesn't match FIRST(H), treat it as the epsilon production in L and do nothing.
    printf("Empty"); //WILL HAVE TO ADD THIS to tokenInstance on the node
      return;
    }
}

void E() {
    // Implement logic based on the production rule E -> , A A H | ,; F H
    if(tokenLookahead.id == T3_tk && tokenLookahead.tokenInstance[0]== ',' && tokenLookahead.tokenInstance[1] != ';') {
    
     tokenLookahead = FADriver(&lineNumber); // Get next token
     A();
     A();
     H();
    }else if(tokenLookahead.id == T3_tk && tokenLookahead.tokenInstance[0]== ',' && tokenLookahead.tokenInstance[1] == ';'){
        
        tokenLookahead = FADriver(&lineNumber); // Get next token
        F();
        H();
    }
}

void F() {
    // Implement logic based on the production rule F -> t1 | t2
    if (tokenLookahead.id == T1_tk){
        //Here add the token into tree and return LATER
        tokenLookahead = FADriver(&lineNumber); // Get next token
        return;
    }else if(tokenLookahead.id == T2_tk){
        //make node and add to tree LATER
        tokenLookahead = FADriver(&lineNumber); // Get next token
        return;
    }else{
        printf("ERROR IN PRODUCTION F: Required t1 or t2 token\n");
    }
}

void G() {
    // Implement logic based on the production rule G -> B | C | J
    if(tokenLookahead.id == T3_tk && tokenLookahead.tokenInstance[0] == '.'){
        B();
    }else if(tokenLookahead.id == T2_tk){
        C();
    }else if(tokenLookahead.id == T3_tk && (tokenLookahead.tokenInstance[0] == "*" && tokenLookahead.tokenInstance[1] == '"')){
        J();
    }
}

void H() {
    // Implement logic based on the production rule H -> E ? | G . | empty
    if(tokenLookahead.id == T3_tk && tokenLookahead.tokenInstance[0] == ',') { //if it's part of E's first set--- E: , | ,;
    tokenLookahead = FADriver(&lineNumber); // Get next token
    E();
    }else if(tokenLookahead.id == T2_tk || 
       tokenLookahead.id == T3_tk && (tokenLookahead.tokenInstance[0] == "." || tokenLookahead.tokenInstance[0] == "*" )){ //G: . | t2 | *"
    
    tokenLookahead = FADriver(&lineNumber); // Get next token
    G();
    }else{
        printf("Empty\n"); //This will have to go into node instance as empty
        return;
    }

}

void J() {
    // Implement logic based on the production rule J -> *” A .
    if(tokenLookahead.id == T3_tk && (tokenLookahead.tokenInstance[0]=="*" && tokenLookahead.tokenInstance[1] == '"' )){
        //process *" in node tree here when implemented
        //
        tokenLookahead = FADriver(&lineNumber); // Get next token
        A();

        if(tokenLookahead.id == T3_tk && tokenLookahead.tokenInstance[0] == "."){
        //process . in node tree here when implemented
            tokenLookahead = FADriver(&lineNumber); //Get next token
            return;
        }else{
            printf("ERROR IN PRODUCTION J: Expected .\n");
            exit(EXIT_FAILURE);
        }
    }
}

void K(){ // K -> F ?$ | .

    if(tokenLookahead.tokenInstance == '.' ){
        //When tree is set up, add into tree instance
        tokenLookahead = FADriver(&lineNumber);
        return;
    }else if(tokenLookahead.id == T1_tk || tokenLookahead.id == T2_tk){
     F();
     if(tokenLookahead.tokenInstance[0] == '?' && tokenLookahead.tokenInstance[1] == '$'){
            tokenLookahead = FADriver(&lineNumber); // Consume "?$"
            return;
        
         }else{
            printf("ERROR in K(): Expected ?$ after F\n");
            exit(EXIT_FAILURE);
        }
    }
}