/* Definitions and some functions implementations
 * parse.cpp to be completed
 * Programming Assignment 2
 * Spring 2022
*/

#include "parse.h"


map<string, bool> defVar;
map<string, Token> SymTable;

namespace Parser {
	bool pushed_back = false;
	LexItem	pushed_token;

	static LexItem GetNextToken(istream& in, int& line) {
		if( pushed_back ) {
			pushed_back = false;
			return pushed_token;
		}
		return getNextToken(in, line);
	}

	static void PushBackToken(LexItem & t) {
		if( pushed_back ) {
			abort();
		}
		pushed_back = true;
		pushed_token = t;
	}

}

static int error_count = 0;

int ErrCount()
{
    return error_count;
}

void ParseError(int line, string msg)
{
	++error_count;
	cout << line << ": " << msg << endl;
}


//Stmt is either a WriteLnStmt, ForepeatStmt, IfStmt, or AssigStmt
//Stmt = AssigStmt | IfStmt | WriteStmt | ForStmt
bool Prog(istream& in, int& line) {
    LexItem t = Parser::GetNextToken(in, line);
    if(t.GetToken() != PROGRAM ){
        ParseError(line, "Missing PROGRAM" );
        return false;
    }
    t = Parser::GetNextToken(in, line);
    if(t.GetToken() != IDENT ){
        ParseError(line, "Missing Program Name");
        return false;
    } else{
        defVar[t.GetLexeme()] = true;
    }
    t = Parser::GetNextToken(in, line);
    if(t.GetToken() != SEMICOL ){
        ParseError(line, "Missing Semicolon: ';' "+ t.GetLexeme());
        return false;
    }
    if(DeclBlock(in, line) == false){
        ParseError(line, "Incorrect Declaration Section.");
        return false;
    }


    if(ProgBody(in, line) == false){
        ParseError(line, "Incorrect Program Body.");
        return false;
    }
    return true;
}
bool DeclBlock(istream& in, int&line){
    LexItem t = Parser::GetNextToken(in, line);
    bool status = true;
    if(t.GetToken() != VAR ){
        ParseError(line, "Missing VAR keyword");
        return false;
    }
    while(status && (t = Parser::GetNextToken(in, line)).GetToken() == IDENT){
        Parser::PushBackToken(t);
        if(DeclStmt(in, line) == false) {
            ParseError(line, "Syntactic error in Declaration Block.");
            status = false;
            break;
        }
        t = Parser::GetNextToken(in, line);
        if(t.GetToken() != SEMICOL ){
            ParseError(line, "Missing Semicolon: ';' " + t.GetLexeme());
            status = false;
        }
    }
    Parser::PushBackToken(t);
    if(!status){
        ParseError(line, "Syntactic error in Declaration Block.");
    }
    return status;
}
bool DeclStmt(istream& in, int&line){
    LexItem t = Parser::GetNextToken(in, line);
    if(defVar[t.GetLexeme()] == true){
        ParseError(line, "Identifier is already used");
        return false;
    } else{
        defVar[t.GetLexeme()] = true;
    }
    while((t = Parser::GetNextToken(in, line)).GetToken() == COMMA){
        t = Parser::GetNextToken(in, line);
        if(t.GetToken() != IDENT ){
            ParseError(line, "Identifier not used in Declaration Statement");
            return false;
        } else if(defVar[t.GetLexeme()] == true){
            ParseError(line, "Identifier is already used");
            return false;
        } else{
            defVar[t.GetLexeme()] = true;
        }
    }
    if(t.GetToken() != COLON ){
        ParseError(line, "Missing colon ':' ");
        return false;
    }
    t = Parser::GetNextToken(in, line);
    bool status = false;
    switch(t.GetToken()) {
        default:
            status = false;
        case ERR:
            ParseError(line, "Incorrect Declaration Type.");
		    return false;
        case INTEGER:
        case REAL:
        case STRING:
            status = true;
            break;
    }
    if(!status) {
        ParseError(line, "Identifier must be declared as an INTEGER, REAL, or STRING");
    }
    return status;
}

bool ProgBody(istream& in, int& line) {
    LexItem t = Parser::GetNextToken(in, line);
    if(t.GetToken() != BEGIN){
        ParseError(line, "Missing BEGIN Keyword");
        return false;
    }
    while(Stmt(in, line) == true) {
        t = Parser::GetNextToken(in, line);
        if(t.GetToken() != SEMICOL){
            ParseError(line-1, "Missing semicolon in Statement.");
            return false;
        }
    }
    t = Parser::GetNextToken(in, line);
    if(t.GetToken() != END){
        ParseError(line, "Syntactic error in Program Body.");
        return false;
    }
    return true;
}

bool Stmt(istream& in, int& line) {
	bool status;
	//cout << "in ContrlStmt" << endl;
	LexItem t = Parser::GetNextToken(in, line);

	switch( t.GetToken() ) {

	case WRITELN:
		status = WriteLnStmt(in, line);
		//cout << "After WriteStmet status: " << (status? true:false) <<endl;
		break;

	case IF:
		status = IfStmt(in, line);
		break;

	case IDENT:
		Parser::PushBackToken(t);
        status = AssignStmt(in, line);

		break;

	case FOR:
		status = ForStmt(in, line);

		break;


	default:
		Parser::PushBackToken(t);
		return false;
	}

	return status;
}//End of Stmt


//WriteStmt:= wi, ExpreList
bool WriteLnStmt(istream& in, int& line) {
	LexItem t;
	//cout << "in WriteStmt" << endl;

	t = Parser::GetNextToken(in, line);
	if( t != LPAREN ) {

		ParseError(line, "Missing Left Parenthesis");
		return false;
	}

	bool ex = ExprList(in, line);

	if( !ex ) {
		ParseError(line, "Missing expression after WriteLn");
		return false;
	}

	t = Parser::GetNextToken(in, line);
	if(t != RPAREN ) {

		ParseError(line, "Missing ) after expression");
		return false;
	}
	//Evaluate: print out the list of expressions values

	return ex;
}

bool IfStmt(istream& in, int& line){
    LexItem t = Parser::GetNextToken(in, line);
    if(t.GetToken() != LPAREN){
        ParseError(line, "Missing Left Parenthesis");
        return false;
    }
    if(LogicExpr(in, line) == false) {
        ParseError(line, "If-Stmt Syntax Error");
        return false;
    }
    t = Parser::GetNextToken(in, line);
    if(t.GetToken() != RPAREN){
        ParseError(line, "Missing Right Parenthesis");
        return false;
    }
    t = Parser::GetNextToken(in, line);
    if(t.GetToken() != THEN){
        ParseError(line, "Missing THEN keyword");
        return false;
    }
    if(Stmt(in, line) == false) {
        ParseError(line, "Missing Statement for If-Stmt Then-Part");
        return false;
    }
    t = Parser::GetNextToken(in, line);
    if(t.GetToken() != ELSE){
        Parser::PushBackToken(t);
        return true;
    }
    if(Stmt(in, line) == false){
        ParseError(line, "Invalid Else Statement Body");
        return false;
    }
    return true;
}

bool ForStmt(istream& in, int& line) {
    if(Var(in, line) == false) {
        return false;
    }
    if(Parser::GetNextToken(in, line).GetToken() != ASSOP){
        ParseError(line, "Missing Assignment Operator");
        return false;
    }
    if(Parser::GetNextToken(in, line).GetToken() != ICONST){
        ParseError(line, "Missing integer constant in For Loop assignment");
        return false;
    }
    switch(Parser::GetNextToken(in, line).GetToken()) {
        case TO:
        case DOWNTO:
            break;
        default:
            ParseError(line, "Missing TO or DOWNTO keyword");
            return false;
    }
    if(Parser::GetNextToken(in, line).GetToken() != ICONST){
        ParseError(line, "Missing ICONST in For Loop condition");
        return false;
    }
    if(Parser::GetNextToken(in, line).GetToken() != DO){
        ParseError(line, "Missing DO keyword in For Loop");
        return false;
    }
    if(Stmt(in, line) == false) {
        ParseError(line, "Invalid For Loop body");
        return false;
    }
    return true;
}

bool AssignStmt(istream& in, int& line) {
    if(Var(in, line) == false) {
        ParseError(line, "Missing Expression in Assignment Statment");
        return false;
    }
    if(Parser::GetNextToken(in, line) != ASSOP) {
        ParseError(line, "Missing assignment operator");
        return false;
    }
    if(Expr(in, line) == false) {
        ParseError(line, "Missing Expression in Assignment Statment");
        return false;
    }
    return true;
}

//ExprList:= Expr {,Expr}
bool ExprList(istream& in, int& line) {
	bool status = false;
	//cout << "in ExprList and before calling Expr" << endl;
	status = Expr(in, line);
	if(!status){
		ParseError(line, "Missing Expression");
		return false;
	}

	LexItem tok = Parser::GetNextToken(in, line);

	if (tok.GetToken() == COMMA) {
		//cout << "before calling ExprList" << endl;
		status = ExprList(in, line);
		//cout << "after calling ExprList" << endl;
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		return false;
	}
	else{
		Parser::PushBackToken(tok);
		return true;
	}
	return status;
}

bool Var(istream& in, int& line) {
    LexItem t = Parser::GetNextToken(in, line);
    if(!defVar[t.GetLexeme()]) {
        ParseError(line, "Invalid identifier");
        return false;
    }
    return true;
}

bool LogicExpr(istream& in, int& line) {
    if(Expr(in, line) == false) {
        return false;
    }
    switch(Parser::GetNextToken(in, line).GetToken()){
        case ERR:
            ParseError(line, "Unrecognized Input Pattern");
		    return false;
        case EQUAL:
        case LTHAN:
        case GTHAN:
            break;
        default:
            ParseError(line, "Invalid logical operator");
            return false;
    }
    if(Expr(in, line) == false) {
        ParseError(line, "Missing expression after logical operator");
        return false;
    }
    return true;
}

bool Expr(istream& in, int& line){
    if(Term(in, line) == false) {
        return false;
    }
    LexItem t;
    while(true) {
        bool exit = false;
        t = Parser::GetNextToken(in, line);
        switch(t.GetToken()){
            case ERR:
                ParseError(line, "Unrecognized Input Pattern");
                return false;
            case PLUS:
            case MINUS:
                break;
            default:
                exit = true;
                break;
        }
        if(exit) {
            break;
        }
        if(Term(in, line) == false) {
            ParseError(line, "Missing term after operator");
            return false;
        }
    }
    Parser::PushBackToken(t);
    return true;
}

bool Term(istream& in, int& line) {
    if(SFactor(in, line) == false) {
        return false;
    }
    LexItem t;
    while(true) {
        bool exit = false;
        t = Parser::GetNextToken(in, line);
        switch(t.GetToken()){
            case ERR:
                ParseError(line, "Unrecognized Input Pattern");
                return false;
            case MULT:
            case DIV:
                break;
            default:
                exit = true;
                break;
        }
        if(exit) {
            break;
        }
        if(SFactor(in, line) == false) {
            ParseError(line, "Invalid term");
            return false;
        }
    }
    Parser::PushBackToken(t);
    return true;
}

bool SFactor(istream& in, int& line) {
    LexItem t = Parser::GetNextToken(in, line);
    int sign = 1;
    switch(t.GetToken()) {
        case ERR:
            ParseError(line, "Unrecognized Input Pattern");
		    return false;
        case PLUS:
            break;
        case MINUS:
            sign = 0;
            break;
        default:
            Parser::PushBackToken(t);
            break;
    }
    if(Factor(in, line, sign) == false) {
        return false;
    }
    return true;
}
bool Factor(istream& in, int& line, int sign) {
    LexItem t = Parser::GetNextToken(in, line);
    switch(t.GetToken()) {
        case ERR:
            ParseError(line, "Unrecognized Input Pattern");
		    return false;
        case IDENT:
            Parser::PushBackToken(t);
            if(Var(in, line) == false) {
                return false;
            }
            break;
        case LPAREN:
            if(Expr(in, line) == false) {
                ParseError(line, "Invalid expression");
                return false;
            }
            if(Parser::GetNextToken(in, line).GetToken() != RPAREN) {
                ParseError(line, "Missing right parenthesis");
                return false;
            }
            break;
        case ICONST:
        case RCONST:
        case SCONST:
            break;
        default:
            return false;
    }
    return true;
}