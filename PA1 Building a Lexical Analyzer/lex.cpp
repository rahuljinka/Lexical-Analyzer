#include "lex.h"
#include <sstream>
#include <string>
#include <algorithm>
#include <iostream>
using namespace std;

static map<Token,string> tokenPrint = {
    {PROGRAM, "PROGRAM"},
    {WRITELN, "WRITELN"},
    {INTEGER, "INTEGER"},
    { END, "END" },
    { IF, "IF" },
    {REAL, "REAL"},
    {STRING, "STRING"},
    { FOR, "FOR" },
    { TO, "TO" },
    { DOWNTO, "DOWNTO" },
    { ELSE, "ELSE" },
    { VAR, "VAR" },
    {BEGIN, "BEGIN"},
    { IDENT, "IDENT" },
    { ICONST, "ICONST" },
    { RCONST, "RCONST" },
    { SCONST, "SCONST" },
    { DO, "DO"},
    { PLUS, "PLUS" },
    { MINUS, "MINUS" },
    { MULT, "MULT" },
    { DIV, "DIV" },
    { ASSOP, "ASSOP" },
    { LPAREN, "LPAREN" },
    { RPAREN, "RPAREN" },
    { COMMA, "COMMA" },
    { EQUAL, "EQUAL" },
    { GTHAN, "GTHAN" },
    { LTHAN, "LTHAN"},
    { SEMICOL, "SEMICOL" },
    { COLON, "COLON"},
    { ERR, "ERR" },
    { DONE, "DONE" },
};

//Keywords mapping
static map<string, Token> kwmap = {
    {"PROGRAM", PROGRAM},
    {"WRITELN", WRITELN},
    {"INTEGER", INTEGER},
    {"END", END },
    { "IF", IF },
    {"REAL", REAL},
    {"STRING", STRING},
    { "FOR", FOR },
    { "TO", TO },
    { "DOWNTO", DOWNTO },
    { "ELSE", ELSE },
    { "VAR", VAR},
    {"BEGIN", BEGIN},
    {"DO", DO},
};

LexItem getNextToken(istream& in, int& linenumber){
    enum TokState{
        START,INID,INSTRING,ININT,INREAL,INCOMMENT,VALIDREAL
    } state = START;
    string lexeme;
    char ch;
    while(in.get(ch)){
        switch(state){
            case START:
                if(ch =='\n'){
                    linenumber++;
                    continue;
                }
                else if(isspace(ch)){
                    continue;
                }
                else if(ch == '('){
                    if(in.peek() == '*'){
                        in.get();
                        state = INCOMMENT;
                        continue;
                    }
                }

                lexeme += ch;
                if(isalpha(ch) || ch == '_'){
                    state = INID;
                }
                else if(isdigit(ch)){
                    state = ININT;
                }
                else if(ch == '\''){
                    state = INSTRING;
                }
                else if(ch == '.'){
                    state = INREAL;
                }
                else{
                    switch(ch){
                        case '+': return LexItem(PLUS, lexeme, linenumber);
                        case '-': return LexItem(MINUS, lexeme, linenumber);
                        case '*': return LexItem(MULT, lexeme, linenumber);
                        case '/': return LexItem(DIV, lexeme, linenumber);
                        case '=': return LexItem(EQUAL, lexeme, linenumber);

                        case '(': return LexItem(LPAREN, lexeme, linenumber);
                        case ')': return LexItem(RPAREN, lexeme, linenumber);
                        case ',': return LexItem(COMMA, lexeme, linenumber);
                        case '>': return LexItem(GTHAN, lexeme, linenumber);
                        case '<': return LexItem(LTHAN, lexeme, linenumber);
                        case ';': return LexItem(SEMICOL, lexeme, linenumber);
                        case ':': 
                            if (in.peek() == '='){
                                in.get();
                                return LexItem(ASSOP, lexeme, linenumber);
                            }
                            else{
                                return LexItem(COLON, lexeme, linenumber);
                            }
                        default: return LexItem(ERR, lexeme, linenumber);

                    }
                }
                break;

            case INID:
                if(isalnum(ch) || ch == '_'){
                    lexeme += ch;
                    continue;
                }	else {
                    if(ch == '\n')
                        linenumber++;
                    else if(!isspace(ch))
                        in.putback(ch);
                    return id_or_kw(lexeme, linenumber);
                }
                break;

            case ININT:
                if(isdigit(ch)){
                    lexeme += ch;
                    continue;
                }
                else if (ch == '.'){
                    lexeme += ch;
                    state= INREAL;
                    continue;
                }
                else {
                    in.putback(ch);
                    return LexItem(ICONST, lexeme, linenumber);
                }
                break;

            case INREAL:
                if(isdigit(ch)) {
                    lexeme += ch;
                    state = VALIDREAL;
                }
                else {
                    return LexItem(ERR, lexeme, linenumber);
                }
                break;

            case VALIDREAL:
                if(isdigit(ch)){
                    lexeme += ch;
                    continue;
                }
                else if(ch == '.') {
                    lexeme += ch;
                    return LexItem(ERR, lexeme, linenumber);
                }
                else {
                    in.putback(ch);
                    return LexItem(RCONST, lexeme, linenumber);
                }
                break;

            case INSTRING:
                if(ch == EOF || ch == '\n'){
                    return LexItem(ERR, lexeme, linenumber);
                } 
                else if (ch == '\''){
                    return LexItem(SCONST, lexeme.substr(1), linenumber);
                }
                else {
                    lexeme += ch;
                    continue;
                }
                break;

            case INCOMMENT:
                if(ch == '\n'){
                    linenumber++;
                }
                else if (ch == '*') {
                    if(in.peek() == ')'){
                        in.get();
                        state = START;
                        continue;
                    }
                }
                if(in.peek() == -1) {
                    return LexItem(ERR, "Missing a comment end delimiters \'*)\'", linenumber);
                }
        }
    }
    return LexItem(DONE, lexeme, linenumber);
}
LexItem id_or_kw(const string& lexeme, int linenum){
    string s = lexeme;
    transform(s.begin(), s.end(), s.begin(), ::toupper);
    if(kwmap.find(s) == kwmap.end()){
        LexItem	result(IDENT, lexeme, linenum);
        return result;
    }

    LexItem result(kwmap[s], lexeme, linenum);
    return result;

}

ostream& operator<< (ostream& out,const LexItem& tok){
    Token t = tok.GetToken();
    out<<tokenPrint[t];
    if(t == IDENT || t== ICONST || t == RCONST || t == SCONST){
        string s= tok.GetLexeme();
        if(t != SCONST)
            transform(s.begin(), s.end(), s.begin(), ::toupper);
        out<<"("<<s<<")";
    }
    return out;
}