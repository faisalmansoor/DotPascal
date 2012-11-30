#ifndef ERROR_H_
#define ERROR_H_

#include<string.h>
#include<stdio.h>
#include"Token.h"

enum ERROR_TYPES
{
	ERR_FATAL_ERR = 2000,					//Fatal
	
	///Lexical Errors	
	ERR_ILLEGAL_CHAR_CONST,			//illegal char const
	ERR_INVALID_CHAR,				//illegal char in input
	ERR_L_CURLY_IN_COMMENT,			//left curly brase in comment
	ERR_EMPTY_CHAR_CONST,			//char const is empty
	ERR_UNKNOWN_ERROR,				//unknown
	ERR_CONST_TOO_LONG,				//Int Const must be -32758 - 32767

	/// Syntex Errors
	ERR_SYNTEX_ERROR
};


class Error
{
private:
	//static char *mFileName;
	int mLineNo;
	int mCharPos;
	int Err_No;
	char InfoString[120];
	Token *mpTok;
	
	Error(int eno, char S[],int lno = 0,int cpos = 0)              // error message sent as a string
	{
		Err_No = eno;
		mLineNo = lno;
		mCharPos = cpos;
		strcpy(InfoString,S);
	}

    Error(int eno,char S,int lno = 0,int cpos= 0)               // error message sent as a char
	{
		Err_No = eno;
		mLineNo = lno;
		mCharPos = cpos;		
		InfoString[0] = S;
		InfoString[1] = 0;
	}
    Error(int eno,int lno = 0,int cpos = 0)	                 // no message
	{
		Err_No = eno;
		mLineNo = lno;
		mCharPos = cpos;		
		InfoString[0] = 0;
	}
	Error(int eno,Token* t)	                 // no message
	{
		Err_No = eno;		
		InfoString[0] = 0;
	}
	Error(int eno,Token* t,char S[])	                 // no message
	{
		Err_No = eno;		
		strcpy(InfoString,S);
		mpTok = t;
	}
public:
	static Error* Create(int eno, char S[],int lno = 0,int cpos = 0)              // error message sent as a string
	{
		Error* pe = new Error(eno, S,lno ,cpos );		
		return pe;
	}

    static Error* Create(int eno,char S,int lno = 0,int cpos= 0)               // error message sent as a char
	{
		Error* pe = new Error(eno,S,lno,cpos);		
		return pe;		
	}
    static Error* Create(int eno,int lno = 0,int cpos = 0)	                 // no message
	{
		Error* pe = new Error(eno,lno,cpos);
		return pe;
	}
	static Error* Create(int eno,Token* t)	                 // no message
	{
		Error* pe = new Error(eno,t);		
		return pe;		
	}
	static Error* Create(int eno,Token* t,char S[])	                 // no message
	{
		Error* pe = new Error(eno,t,S);		
		return pe;				
	}
	char* getMessage(void)                   // returns error message
	{
		switch ( Err_No )
		{
		case ERR_L_CURLY_IN_COMMENT:
			return ">>> Error : Cannot include { inside a comment" ;
			break;
			
		case ERR_INVALID_CHAR:
			return strcat(">>> Error: Illegal character : ", InfoString );
			break;
		case ERR_EMPTY_CHAR_CONST:
			return ">>> Error: Empty char constant";
			break;

		}
		return "Unknown error";

	}
    void Dump(FILE* fp = stdout) // prints the current line 		
	{
		//  printf("ERROR NO : %d : LINE NO. %d : %s\n",Err_No,mLineNo,InfoString);
		switch ( Err_No )
		{
		case ERR_L_CURLY_IN_COMMENT:
			fprintf(fp,"ERR_L_CURLY_IN_COMMENT %s : Line No: %d\n",InfoString,mLineNo) ;
			break;
			
		case ERR_INVALID_CHAR:
			fprintf(fp,"ERR_INVALID_CHAR %s : Line No: %d\n",InfoString,mLineNo);
			break;
		case ERR_EMPTY_CHAR_CONST:
			fprintf(fp,"ERR_EMPTY_CHAR_CONST %s : Line No: %d\n",InfoString,mLineNo);
			break;
		case ERR_CONST_TOO_LONG:
			fprintf(fp,"ERR_CONST_TOO_LONG %s : Line No: %d\n",InfoString,mLineNo);
			break;

		case ERR_SYNTEX_ERROR:
			fprintf(fp,"%s : Line No: %d\n",InfoString,mLineNo);
			break;
		}		
	}
};

//char* Error::mFileName = "";

#endif

