/*
* Copyright (C) Rida Bazzi, 2017
*
* Do not share this file with anyone
*/
#ifndef __LEXER__H__
#define __LEXER__H__

#include <vector>
#include <string>

#include "inputbuf.h"

// Lexer modified for FIRST & FOLLOW project

typedef enum { END_OF_FILE = 0, ARROW, HASH, DOUBLEHASH, ID, ERROR } TokenType;

class Token {
public:
    void Print();

    std::string lexeme;
    TokenType token_type;
    int line_no;
};
class Rule {
public:
    void Print();
    
    Token leftHand;
    std::vector<Token> rightHand;
};

class LexicalAnalyzer {
public:
    Token GetToken();
    TokenType UngetToken(Token);
    LexicalAnalyzer();
    bool Parse();
    void PrintRules();
    void ParseTerminals();
    void ParseGenerating();
    void PrintTaskOne();
    void PrintGenerating();
    void PrintReachable();
    void CreateFirstSets();
    bool SearchFound(Token);
    void PrintFirstSets();
    void CreateFollowSets();
    int NameIndex(std::string);
    

private:
    std::vector<Token> tokens;
    std::vector<std::vector<Token> > firstSets;
    std::vector<std::vector<Token> > followSets;
    std::vector<bool>generatingTokens;
    std::vector<Rule>generatingRules;
    std::vector<std::string> nameVector;
    std::vector<Rule> ruleVector;
    std::vector<Token> nonTerminals;
    std::vector<Token> terminals;
    std::vector<Rule> reachableRules;
    std::vector<int> reachableTokens;
    std::vector<Token> foundTokens;
    int line_no;
    Token tmp;
    InputBuffer input;

    bool SkipSpace();
    Token ScanId();
};

#endif  //__LEXER__H__