#ifndef GLOBAL_H
#define GLOBAL_H


const IDENTIFIER_LEN = 63;

struct Bounds
{
	int lb;
	int ub;
	bool operator != ( Bounds& b)
	{
		return ! ( *this == b );
	}
	bool operator == (Bounds& b)
	{
		return ( lb == b.lb && ub == b.ub );
	}
};

enum OP_TYPE
{
	OP_le = 3000, OP_ne, OP_lt,
	OP_eq,
	OP_ge, OP_gt,
	OP_plus, OP_minus, OP_or,
	OP_mul, OP_div , OP_mod, OP_and	
};

enum CONST_TYPE
{
	CONST_integer = 4000,
	CONST_real,
	CONST_string,
	CONST_char
};

enum TOKEN_TYPE
{  
   T_null = -1,
   T_program = 5000,// Program keyword
   T_begin,			// Begin keyword
   T_end,			// End    
   T_var,			// Var keyword
   T_function,		// Function keyword
   T_procedure,		// Procedure keyword
   T_integer,		// Integer keyword
   T_real,			// Real keyword
   T_array,			// Array keyword
   T_of,			// Of keyword
   T_if,			// If keyword
   T_then,			// Then keyword
   T_else,			// Else keyword
   T_while,			// While keyword
   T_do,			// Do keyword
   T_not,			// Not keyword
   T_case,			// case Keyword
   T_default,
   T_for,			// for Keyword
   T_to,			// For for loop
   T_downto,		// For for loop
   T_step,			// Step			
   T_identifier,	// Identifier
   T_constant,		// Constant (integer or real number or char or string)   
   T_relop,			// Relop("=", "<> ", "< ", "> ", "< =", "> =" )
   T_mulop,			// Mulop("*", "/", "div","mod", "and")
   T_addop,			// Addop("+", "-", "or")
   T_assignop,		// Assignment operator (":=")
   T_comma,			// Comma
   T_semicolon,		// Semicolon
   T_colon,			// Colon
   T_rightparen,	// Right parenthesis 
   T_leftparen,		// Left parenthesis  
   T_rightbracket,	// Right bracket	 ]	
   T_leftbracket,	// Left bracket	     [		
   T_unaryminus,	// Unary minus
   T_unaryplus,		// Unary plus
   T_doubledot,		// Doubledot ("..")
   T_endmarker,		// End of program marker (".")
   T_eof			// End of file marker   
};

enum EXP_TYPE
{
	ExpVoid = 6000,
	ExpInt,
	ExpReal,
	ExpArray,
	ExpError
};

enum VAR_TYPE
{
	VAR_SIMPLE,
	VAR_TEMP,
	VAR_PARAM,	
	VAR_RETURN_VAL,	
	VAR_LOCAL,
	VAR_STATIC,
	VAR_INT,
	VAR_ARRAY,
	VAR_REAL,
	VAR_STRING,
	VAR_FUNCTION,
	VAR_PROCEDURE,
	VAR_CONSTANT,
	VAR_ARGUMENT,
	VAR_ERR

};

static int gKeyCount = 20;	
static const char* gKeyWords[]  = {
	"PROGRAM",
		"BEGIN",			// Begin keyword
		"END",				// End 
		"VAR",				// Var keyword
		"FUNCTION",			// Function keyword
		"PROCEDURE",		// Procedure keyword
		"INTEGER",			// Integer keyword
		"REAL",				// Real keyword
		"ARRAY",			// Array keyword
		"OF",				// Of keyword
		"IF",				// If keyword
		"THEN",				// Then keyword
		"ELSE",				// Else keyword
		"WHILE",			// While keyword
		"DO",				// Do keyword
		"NOT",				// Not keyword
		"CASE",				// Case keyword
		"FOR",   			// for Keyword
		"TO",
		"DOWNTO"
};
static int gKeyId[] = {
	T_program,
		T_begin,
		T_end,
		T_var,
		T_function,
		T_procedure,
		T_integer,
		T_real,
		T_array,
		T_of,
		T_if,
		T_then,
		T_else,
		T_while,
		T_do,
		T_not,
		T_case
};

#endif
