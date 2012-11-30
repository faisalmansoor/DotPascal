#include<stdlib.h>
#include "error.h"
#include "CharStream.h"


CharStream::CharStream(char* filename )
{
	mLineNo = 0;
	mCurrChar = 0;
	mInString = false;
	mfp = fopen(filename,"r");
	if ( !mfp )
	{
		printf("Cant Open file %s \nExitting......\n",filename);
		exit(0);
	}
	GetNewLine();
}
void CharStream::SkipWhite()
{
	if ( mCurrLine[mCurrChar] == L_CURLY )
		SkipComment();
	
	while ( (mCurrLine[mCurrChar] == BLANK) ||
		(mCurrLine[mCurrChar] == TAB  ) ||
		(mCurrLine[mCurrChar] == EOL_CHAR ) ||
		(mCurrLine[mCurrChar] == NL_CHAR ) 				
		)
	{
		Advance();
		
		if ( mCurrLine[mCurrChar] == L_CURLY )
			SkipComment();
	}
	
}
void CharStream::Advance()
{
	if ( mCurrLine[mCurrChar] == EOL_CHAR )
		GetNewLine();
	
	else ++mCurrChar;
}
void CharStream::SkipComment()
{
	while ( mCurrLine[mCurrChar] != R_CURLY )
	{
		Advance();
		
		if ( mCurrLine[mCurrChar] == L_CURLY )
		{
			Error::Create(ERR_INVALID_CHAR,mCurrLine[mCurrChar]);			
		}				
	}
	
	Advance();		
}
void CharStream::GetNewLine()
{
	if( fgets(mCurrLine,256,mfp) )
	{
		++mLineNo;
		mCurrChar = 0;
		//strupr(mCurrLine);
		SkipWhite();
	}
	else
	{
		mCurrChar = 0;
		mCurrLine[mCurrChar] = EOF_CHAR;
	}
}	
bool CharStream::Valid (char c)
{
	bool result ;
	result = ( c == '\n' );
	result = result || ( c == EOL_CHAR);
	result = result || ( strchr(VALID_CHARS,c) != NULL);
	
	return result;
}
char CharStream::GetNextChar() 
{
	if ( eof () )
		return EOF_CHAR;

	char c = CurrChar();
	
	while ( Valid(c) == false )
	{
		Error::Create(ERR_INVALID_CHAR,c,mLineNo);
		c = CurrChar();
	}
	return c;
}	

char CharStream::CurrChar()
{
	
	char c;
	
	c = ( mCurrLine[mCurrChar] );		
	
	if ( c == EOL_CHAR || c == NL_CHAR )
	{
		GetNewLine();
		while ( (mCurrLine[mCurrChar] == EOL_CHAR) ||
			(mCurrLine[mCurrChar] == NL_CHAR) 
			)
			// handle multiple blank lines
			GetNewLine();
		
		return(BLANK);
	}
	
	if ( c == L_CURLY )
	{
		SkipComment();			
		return(BLANK);
	}
	
	if ( c == BLANK || c == TAB )                        
		// First blank is returned as a delimeter
	{
		SkipWhite();			
		return(BLANK);
	}
	
	Advance();

	if( c == '\'')
		mInString = !mInString;

	if( mInString == false )
		c = toupper(c);
	
	return (c);
}

