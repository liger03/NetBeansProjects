/*
* Copyright (C) Rida Bazzi, 2017
*
* Do not share this file with anyone
*/
#include <iostream>
#include <istream>
#include <vector>
#include <string>
#include <cctype>

#include "lexer.h"
#include "inputbuf.h"

using namespace std;

// Lexer modified for FIRST & FOLLOW project

string reserved[] = { "END_OF_FILE", "ARROW", "HASH", "DOUBLEHASH", "ID", "ERROR" };

void Token::Print()
{
    cout << "{" << this->lexeme << " , "
            << reserved[(int)this->token_type] << " , "
            << this->line_no << "}\n";
}
void Rule::Print()  //print a rule
{
    cout<<leftHand.lexeme<<" -> ";
    if(rightHand.empty())
        cout<<"#";
    else
        for(int i=0; i<rightHand.size();i++)
            cout<<rightHand[i].lexeme<<" ";
    cout<<endl;
}

LexicalAnalyzer::LexicalAnalyzer()
{
	this->line_no = 1;
	tmp.lexeme = "";
	tmp.line_no = 1;
	tmp.token_type = ERROR;
}

bool LexicalAnalyzer::Parse()   //parses a full input, returns false on failure and true on success
{
    string inputStr;
    Token inputToken;
    bool firstRun=true;
    while(true)
    {
        Rule newRule;
        getline(cin, inputStr);
        if(firstRun)
        {
            firstRun=false;
            input.UngetChar(' ');       //no idea why, but this code eats a single character on the second rule without this.
        }
        input.UngetString(inputStr);
        inputToken=GetToken();
        switch(inputToken.token_type)
        {
            case DOUBLEHASH:    //end of input rules
                ParseTerminals();
                return true;
            case ID:            //start of a new rule
                newRule.leftHand=inputToken;  //get the first token of the rule
                if(!SearchFound(inputToken))
                    foundTokens.push_back(inputToken);
                if(GetToken().token_type!=ARROW)    //if it isn't an arrow next,
                {
                    cout<<"ERROR! Incomplete rule!";
                    return false;   //error out
                }
                inputToken=GetToken();  //now parse the right hand tokens
                while(inputToken.token_type!=HASH)  //until the rule is finished
                {
                    if(!SearchFound(inputToken))
                        foundTokens.push_back(inputToken);
                    newRule.rightHand.push_back(inputToken);    //add the next token to the rule
                    inputToken=GetToken();
                }
                if(!SearchFound(inputToken))
                    foundTokens.push_back(inputToken);          //we even determine what's found before a hash
                ruleVector.push_back(newRule);  //after reaching the hash, add the rule to the list of rules
                break;
            default:
                cout<<"Error! Invalid start of rule."<<endl;
                return false;
        }
    }
}

void LexicalAnalyzer::PrintRules()
{
    for(int i=0; i<ruleVector.size(); i++)
        ruleVector[i].Print();
}
void LexicalAnalyzer::ParseTerminals()
{
    for(int i=0; i<ruleVector.size(); i++)
    {
        bool found=false;
        for(int j=0; j<nonTerminals.size(); j++)
        {
            if(nonTerminals[j].lexeme==ruleVector[i].leftHand.lexeme)
                found=true;     //if it's already a non terminal, don't re-add it.
        }
        if(!found)
            nonTerminals.push_back(ruleVector[i].leftHand);
    }
    for(int i=0; i<ruleVector.size(); i++)
    {
        for(int j=0; j<ruleVector[i].rightHand.size(); j++)
        {
            bool found=false;
            for(int k=0; k<terminals.size(); k++)
            {
                if(ruleVector[i].rightHand[j].lexeme==terminals[k].lexeme)
                    found=true;
            }
            if(!found)
            {
                for(int k=0; k<nonTerminals.size(); k++)
                {
                    if(ruleVector[i].rightHand[j].lexeme==nonTerminals[k].lexeme)
                        found=true;
                }
            }
            if(!found)
                terminals.push_back(ruleVector[i].rightHand[j]);
        }
    }
}
void LexicalAnalyzer::ParseGenerating() //parses all generating and reachable rules
{
    //first, set up the first/follow vector and name vector.
    //all terminals are generating, epsilon is generating,
    //and eof is not generating.
    //Those will be set up on initialization.
    nameVector.clear();
    generatingTokens.clear();
    nameVector.push_back("#");
    generatingTokens.push_back(true);
    nameVector.push_back("$");
    generatingTokens.push_back(false);
    for(int i=0; i<terminals.size(); i++)
    {
        nameVector.push_back(terminals[i].lexeme);
        generatingTokens.push_back(true);
    }
    //non terminals will be tested, first assume false
    for (int i=0; i<nonTerminals.size(); i++)
    {
        nameVector.push_back(nonTerminals[i].lexeme);
        generatingTokens.push_back(false);
    }
    bool repeat=false;
    bool generatingSoFar;
    do
    {
        repeat=false;
        for(int i=0; i<ruleVector.size(); i++)
        {
            generatingSoFar=true;
            for(int j=0; j<ruleVector[i].rightHand.size(); j++)
            {
                //this part took a while to write properly.
                //if  the generating bit vector's index corresponding to the token
                //is currently false, that token is not generating and thus the currently
                //checked rule cannot be said to be generating yet.
                if(generatingTokens[NameIndex(ruleVector[i].rightHand[j].lexeme)]==false)
                {
                    generatingSoFar=false;
                }
            }
            if(generatingSoFar)
            {
                //if a rule fully passes the above loop, all of the right hand tokens
                //should be generating and thus the left-hand token is generating too.
                //this also works on epsilon! Definitely isn't a bug, totally a feature.
                //if this is a newly found generating token, the whole block of code must run again.
                if(generatingTokens[NameIndex(ruleVector[i].leftHand.lexeme)]==false)
                {
                    repeat=true;
                    generatingTokens[NameIndex(ruleVector[i].leftHand.lexeme)]=true;
                }
            }
        }
    }while(repeat);
    //now we create a set of generating rules to work with
    bool skipRule;
    for(int i=0; i<ruleVector.size(); i++)
    {
        skipRule=false;
        if(generatingTokens[NameIndex(ruleVector[i].leftHand.lexeme)]==false)
        {
            skipRule=true;
        }
        for(int j=0; j<ruleVector[i].rightHand.size(); j++)
        {
            if(generatingTokens[NameIndex(ruleVector[i].rightHand[j].lexeme)]==false)
            {
                skipRule=true;
            }
        }
        if(skipRule==false)
        {
            generatingRules.push_back(ruleVector[i]);
        }
    }
    for (int i=0; i<generatingTokens.size(); i++)
    {
        if(generatingTokens[i]==false)
            reachableTokens.push_back(-1);      //non-generating tokens are NEVER reachable, 
        else                                    //so we use -1 to prevent making them reachable
            reachableTokens.push_back(0);
    }
    reachableTokens[NameIndex(ruleVector[1].leftHand.lexeme)]=1; //first symbol is always reachable
    do
    {
        repeat=false;
        for(int i=0; i<generatingRules.size();i++)
        {
            if(reachableTokens[NameIndex(generatingRules[i].leftHand.lexeme)]==1)
            {
                for(int j=0; j<generatingRules[i].rightHand.size(); j++)
                {
                    if(reachableTokens[NameIndex(generatingRules[i].rightHand[j].lexeme)]==0)
                    {
                        repeat=true;
                        reachableTokens[NameIndex(generatingRules[i].rightHand[j].lexeme)]=1;
                    }
                }
            }
        }
        
    }while(repeat);
        //and now to create a set of non-useless rules!
    for(int i=0; i<ruleVector.size(); i++)
    {
        skipRule=false;
        if(reachableTokens[NameIndex(ruleVector[i].leftHand.lexeme)]!=1)
        {
            skipRule=true;
        }
        for(int j=0; j<ruleVector[i].rightHand.size(); j++)
        {
            if(reachableTokens[NameIndex(ruleVector[i].rightHand[j].lexeme)]!=1)
            {
                skipRule=true;
            }
        }
        if(skipRule==false)
        {
            reachableRules.push_back(ruleVector[i]);
        }
    }
}

void LexicalAnalyzer::PrintTaskOne()
{
    for(int j=0; j<foundTokens.size(); j++)
    {
        for(int i=0; i<terminals.size();i++)
        {
            if(terminals[i].lexeme==foundTokens[j].lexeme)
            {
                cout<<terminals[i].lexeme<<" ";
            }
        }
    }
    for(int j=0; j<foundTokens.size(); j++)
    {
        for(int i=0; i<nonTerminals.size(); i++)
        {
            if(nonTerminals[i].lexeme==foundTokens[j].lexeme)
            {
                cout<<nonTerminals[i].lexeme<<" ";
            }
        }
    }
    cout<<endl;
}
void LexicalAnalyzer::PrintGenerating()
{
    for(int i=0; i<generatingRules.size(); i++)
        generatingRules[i].Print();
}
void LexicalAnalyzer::PrintReachable()
{
    for(int i=0; i<reachableRules.size();i++)
        reachableRules[i].Print();
}

void LexicalAnalyzer::CreateFirstSets() //firstVector[0] is the nonterminal, x>0 is the first set of the terminal
{
    for(int i=0; i<nonTerminals.size(); i++)    //for all non terminals..
    {
        firstSets.push_back(vector<Token> () );
        firstSets[i].push_back(nonTerminals[i]);
        for(int j=0; j<ruleVector.size(); j++)  //for all rules in the rule vector
        {
            if(ruleVector[j].leftHand.lexeme==firstSets[i][0].lexeme) //in rules where the left hand lexeme 
            {                                                           //matches the first set we're using
                bool stillAdd=true;
                if(ruleVector[j].rightHand.empty())                     //if the right hand rule is empty
                {
                    for(int k=1; k<firstSets[i].size(); k++)
                    {
                        if(firstSets[i][k].lexeme=="#")               //and there's no entries of # in the first set
                            stillAdd=false;
                    }
                    if(stillAdd)
                    {
                        Token hashToken;
                        hashToken.lexeme="#";
                        hashToken.token_type=HASH;
                        firstSets[i].push_back(hashToken);            //create and add a # entry
                    }
                }
                else                                                    //if the right hand rule isn't empty
                {
                    for(int k=1; k<firstSets[i].size(); k++)
                    {
                        if(ruleVector[j].rightHand[0].lexeme==firstSets[i][k].lexeme) //and the first lexeme of the right-hand
                            stillAdd=false;                                             //isn't part of the current first set
                    }
                    if(stillAdd)
                    {
                        firstSets[i].push_back(ruleVector[j].rightHand[0]);       //add it to the first set.
                    }
                }
            }
        }
        
    }
}
bool LexicalAnalyzer::SearchFound(Token target) //checks if a token has been found or not
{
    for(int i=0; i<foundTokens.size(); i++)
        if(foundTokens[i].lexeme==target.lexeme)
            return true;
    return false;
}

void LexicalAnalyzer::PrintFirstSets()
{
    for(int i=0; i<firstSets.size(); i++)
    {
        cout<<"FIRST("<<firstSets[i][0].lexeme<<") = {";
        cout<<" "<<firstSets[i][1].lexeme;
        for(int j=2; j<firstSets[i].size();j++)
        {
            cout<<", "<<firstSets[i][j].lexeme;
        }
        cout<<" }"<<endl;
    }
}

void LexicalAnalyzer::CreateFollowSets()
{
    
}

int LexicalAnalyzer::NameIndex(string name)    //returns index of matching lexeme in vector, or -1 if not a name
{
    for(int i=0; i<nameVector.size(); i++)
    {
        if(name==nameVector[i])
            return i;
    }
    return -1;
}

bool LexicalAnalyzer::SkipSpace()
{
	char c;
	bool space_encountered = false;

	input.GetChar(c);
	line_no += (c == '\n');

	while (!input.EndOfInput() && isspace(c)) {
		space_encountered = true;
		input.GetChar(c);
		line_no += (c == '\n');
	}

	if (!input.EndOfInput()) {
		input.UngetChar(c);
	}
	return space_encountered;
}

Token LexicalAnalyzer::ScanId()
{
	char c;
	input.GetChar(c);

	if (isalpha(c)) {
		tmp.lexeme = "";
		while (!input.EndOfInput() && isalnum(c)) {
			tmp.lexeme += c;
			input.GetChar(c);
		}
		if (!input.EndOfInput()) {
			input.UngetChar(c);
		}
		tmp.line_no = line_no;
		tmp.token_type = ID;
	}
	else {
		if (!input.EndOfInput()) {
			input.UngetChar(c);
		}
		tmp.lexeme = "";
		tmp.token_type = ERROR;
	}
	return tmp;
}

// you should unget tokens in the reverse order in which they
// are obtained. If you execute
//
//    t1 = lexer.GetToken();
//    t2 = lexer.GetToken();
//    t3 = lexer.GetToken();
//
// in this order, you should execute
//
//    lexer.UngetToken(t3);
//    lexer.UngetToken(t2);
//    lexer.UngetToken(t1);
//
// if you want to unget all three tokens. Note that it does not
// make sense to unget t1 without first ungetting t2 and t3
//
TokenType LexicalAnalyzer::UngetToken(Token tok)
{
	tokens.push_back(tok);;
	return tok.token_type;
}

Token LexicalAnalyzer::GetToken()
{
	char c;

	// if there are tokens that were previously
	// stored due to UngetToken(), pop a token and
	// return it without reading from input
	if (!tokens.empty()) {
		tmp = tokens.back();
		tokens.pop_back();
		return tmp;
	}

	SkipSpace();
	tmp.lexeme = "";
	tmp.line_no = line_no;
	input.GetChar(c);
	switch (c) {
	case '-':
		input.GetChar(c);
		if (c == '>') {
			tmp.token_type = ARROW;
		}
		else {
			if (!input.EndOfInput()) {
				input.UngetChar(c);
			}
			tmp.token_type = ERROR;
		}
		return tmp;
	case '#':
		input.GetChar(c);
		if (c == '#') {
			tmp.token_type = DOUBLEHASH;
		}
		else {
			if (!input.EndOfInput()) {
				input.UngetChar(c);
			}
			tmp.token_type = HASH;
		}
		return tmp;
	default:
		if (isalpha(c)) {
			input.UngetChar(c);
			return ScanId();
		}
		else if (input.EndOfInput())
			tmp.token_type = END_OF_FILE;
		else
			tmp.token_type = ERROR;

		return tmp;
	}
}
