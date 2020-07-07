/*
  Simple integer arithmetic calculator according to the following BNF
  exps		--> exp | exp NEWLINE exps
  exp		--> term {addop term}
  addop		--> + | -
  term		--> factor {mulop factor}
  mulop		--> * | /
  factor	--> ( exp ) | INT
*/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include "tokens.h"
#include "FlexLexer.h"

using namespace std;

string toknames[] = {
"INT", "LPAREN", "RPAREN", "PLUS", "MINUS", "TIMES", "DIVIDE", "NEWLINE"
};

string tokname(int tok) {
  return tok<257 || tok>264 ? "BAD_TOKEN" : toknames[tok-257];
}

yyFlexLexer			lexer;
YYSTYPE				yylval;

int		nextToken;	//global variable stores the token to be processed

void readNextToken( void ); //read the next token into the variable nextToken

void exps( void );	//process all expressions in the input
int  exp( void );	//returns the integer value of an expression
int term ( void );	//returns the integer value of an term
int factor( void );	//returns the integer value of an factor

//If the next token matches expectedToken, read the next token and return true
//otherwise, print an error message and return false
bool match( int expectedToken );

//print the error message
void error( string errorMsg );

//skip the rest of the line
void skipline( void );

int main(int argc, char **argv) {
	ifstream	ifs; 
	
	if (argc!=2) 
	{
		cerr << "usage: " << argv[0] << " filename" << endl;
		return 1;	
	}
	ifs.open( argv[1] );
	if( !ifs ) 
	{
		cerr << "Input file cannot be opened.\n";
        return 0;
	}
	cout << "Lexcial Analysis of the file " << argv[1] << endl;
	
	lexer.switch_streams(&ifs, NULL);

	readNextToken();

	exps();

	return 0;
}

//print the error message, and
//terminate the program
void error( string errorMsg )
{
	cout << errorMsg << endl;
	exit(1);
}

//skip the rest of the line
void skipline( void )
{
	while( nextToken != NEWLINE && nextToken != 0 )
		readNextToken();
}


//read the next token into the variable nextToken
void readNextToken( void )
{
	nextToken = lexer.yylex();
}

//If the next token is expectedToken, call readNextToken and return true,
//otherwise, print an error message and return false
bool match( int expectedToken )
{
	if ( expectedToken == nextToken )
	{
		readNextToken();
		return true;
	}
	else
		return false;
}

void exps( void )
{
	int		count = 1;
	int		value;

	do 
	{
		try {
			value = exp();
			cout << "expression " << count << " : " << value << endl;
		} catch(runtime_error e) {
			skipline();
			cout << "expression " << count << " :    wrong syntax -- " << e.what() << endl;
		}
		count ++;
	} while ( match(NEWLINE) );
}

//**TIPS use tokname function to get name of token
//		 use yylval.ival to get INT value for the token
//returns the integer value of an expression
//exp--> term {addop term}
//The production for exp() contains {} indicating a 0 or more times loop which is why a while loop encloses the 2 ADDOP terminals
int exp( void )
{
	//PUT YOUR IMPLEMENTATION HERE

	int temp = term();
	while ((nextToken == PLUS) || (nextToken == MINUS))
	{
		switch (nextToken) {
		case PLUS: match(PLUS);
			temp += term();
			break;
		case MINUS: match(MINUS);
			temp -= term();
			break;
		}
	}
	
	return temp;
}

//term--> factor {mulop factor}
//The production for term() contains {} indicating a 0 or more times loop which is why a while loop encloses the 2 Mulop terminals
int term ( void )
{
	//PUT YOUR IMPLEMENTATION HERE
	int temp = factor();
	while ((nextToken == DIVIDE) || (nextToken == TIMES))
	{
		switch (nextToken) {
		case TIMES:
			match(TIMES);
			temp *= factor();
			break;
		case DIVIDE:
			match(DIVIDE);
			temp /= factor();
			break;
		}
	}

	return temp;
}

//factor--> ( exp ) | INT
// this production contains the choice operator indicating that there is no loop needed
// factor will either consume a '(' terminal and recurse back to exp() followed by a ')', will have parsed down to an integer value,
// or result in a syntax error which will be handled by throwing a runtime error
//it was not parsing correctly until the RPAREN check was included in LPAREN chek because it was causing the recursion to stop at the LPAREN check
//also returning exp() for LPAREN was causing it to fail at case 3 because it never was reaching base case. 
int factor( void )
{

	int temp;	

	if (nextToken == INT) {
		temp = yylval.ival;
		readNextToken();
		
	}
	else if (nextToken == LPAREN) {
		match(LPAREN);
		temp = exp();
		if (!match(RPAREN)) {
			throw runtime_error("Token RPAREN expected!");
		}

	}
	else if (nextToken == RPAREN)
	{
		return exp();
	}
	else
	{
		throw runtime_error("Token LPAREN or INT expected!");
	}
	
	return temp;	//returning value from if-else-stmts
}
