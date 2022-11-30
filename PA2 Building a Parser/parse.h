/* 
 * parse.h
 * Programming Assignment 2
 * Spring 2022
*/

#ifndef PARSE_H_
#define PARSE_H_

#include <iostream>

using namespace std;

#include "lex.h"



extern bool Prog(istream& in, int& line);
extern bool DeclBlock(istream& in, int& line);
extern bool DeclStmt(istream& in, int& line);
extern bool ProgBody(istream& in, int& line);
extern bool Stmt(istream& in, int& line);
extern bool WriteLnStmt(istream& in, int& line);
extern bool IfStmt(istream& in, int& line);
extern bool ForStmt(istream& in, int& line);
extern bool AssignStmt(istream& in, int& line);
extern bool Var(istream& in, int& line);
extern bool ExprList(istream& in, int& line);
extern bool LogicExpr(istream& in, int& line);
extern bool Expr(istream& in, int& line);
extern bool Term(istream& in, int& line);
extern bool SFactor(istream& in, int& line);
extern bool Factor(istream& in, int& line, int sign);
extern int ErrCount();

#endif /* PARSE_H_ */
