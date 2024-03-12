#ifndef TOKEN_H
#define TOKEN_H

enum tokenID {
    EOF_tk,
    T1_tk,
    T2_tk,
    T3_tk,
    ERROR_NUMBER_START, // ERROR CAN'T START WITH NUMBER
    ERROR_SEMICOLON_START, // ERROR CAN'T START WITH ;
    ERROR_DOLLAR_START, // ERROR CAN'T START WITH $
    ERROR_QUOTE_START, // ERROR CAN'T START WITH "
    ERROR_MUST_BE_NUMBER, // MUST BE NUMBER
    };  // Token IDs

static const char *tokenNames[] = {
"END OF TOKEN",
"T1 token",
"T2 token",
"T3 token",
"ERROR: CANNOT START WITH NUMBER",
"ERROR: CANNOT START WITH SEMICOLON",
"ERROR: CANNOT START WITH DOLLAR SIGN",
"ERROR: CANNOT START WITH QUOTATION MARK",
"ERROR: MUST BE NUMBER"
 };

 struct token {
    enum tokenID id;
    char* tokenInstance ;
    int lineNumber;
 };

#endif
