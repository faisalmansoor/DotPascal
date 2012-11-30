#ifndef SIMPLE_TOKENIZER_H
#define SIMPLE_TOKENIZER_H

#include "CacheCharStream.h"
#include "global.h"
#include "error.h"
#include "ErrManager.h"
#include "ctype.h"
#include "string.h"
#include "Token.h"
#include "KeyWordMap.h"


#define CACHE_LIMIT 256
#define IDNTIFIER_LEN 63
const char DELIM_CHARS[] = ".,;:<>/*[]+-=(){}\' ";

class SimpleTokenizer
{
private:
	CacheCharStream mCS;	
	char mszTok[256];
	Token mPreToken;

	KeyWordMap mKeyMap;

	int mErrBit;
public:
	SimpleTokenizer (char* filename) : mCS(filename,CACHE_LIMIT)
	{
		mErrBit = 0;				// NO Err at the start
	}
	int isdelim(char ch)
	{
		return ( strchr(DELIM_CHARS,ch) != NULL );
	}
	void GetNextToken(Token& t)
	{
		int id;
		mCS.ClearCache();
		if ( IsEOF() )
		{
			t.Set(T_eof,0);
		}
		////NUMBER////////////////////////////
		/////////////////////////////////////
		else if( IsRealConst1() )
		{
			t.Set(T_constant,CONST_real);
		}
		else if( IsRealConst2() )
		{
			t.Set(T_constant,CONST_real);
		}				
		else if( IsIntegerConst() )
		{
			if( mErrBit == 0)
			{
				t.Set(T_constant,CONST_integer);
			}
			else
				mErrBit = 0;
		}
		/////////////////////////////////////////////
		else if( IsDoubleDot() )
		{
			t.Set(T_doubledot,T_null);		
		}
		
		else if( IsIdentifier() )
		{
			int id = -1;
			id = mKeyMap.Get(mszTok);
			if( id != -1)
			{
				t.Set(id,T_null);
			}
			else
			{				
				t.Set(T_identifier,T_null);
			}
		}
		else if( IsCharConst() )
		{
			t.Set(T_constant,CONST_char);
		}
		else if( IsStringConst() )
		{
			t.Set(T_constant,CONST_string);
		}
		else if( id = IsRelOp() )
		{
			t.Set(T_relop,id);
		}
		else if( id = IsAddOp() )
		{
			/*if( id == T_unaryminus || id == T_unaryplus)
				t.Set(id);
			else*/
				t.Set(T_addop,id);				
		}
		else if( id = IsMulOp() )
		{
			t.Set (T_mulop,id);			
		}
		else if( id = IsAssignOp() )
		{
			t.Set (T_assignop,0);
		}
		////
		else if( id = IsOtherSymbol() )
		{
			t.Set(id);			
		}				
		else if( IsBlank() )
		{
			t.Set(T_null);						
		}
		else
		{
			mCS.GetNextChar();
			t.Set(T_null);
		}

		t.LineNo = mCS.LineNo();
		t.Name    = mszTok;
		
		if( t.Class != T_null)
			mPreToken = t;	
	}
	Token GetNextToken()
	{
		Token t;
		GetNextToken(t);
		return t;		
	}
	
	void fail(int count)
	{
		mCS.Retreat(count);
	}
	////////////////////////////////////////
	int IsEOF()
	{
		char ch = mCS.GetNextChar();
		if( ch == EOF_CHAR )
			return true;
		else
		{
			fail(1);
			return false;
		}
	}
	int IsBlank()
	{
		char ch = mCS.GetNextChar();
		if( ch == ' ')
			return true;
		else
		{
			mCS.Retreat();
			return false;
		}		
	}
	int IsOtherSymbol()
	{
		int count = 0;
		char ch = mCS.GetNextChar();
		mszTok[count++] = ch;
		switch(ch)
		{
		case ',':
			mszTok[count++] = 0;
			return T_comma;
		case ';':
			mszTok[count++] = 0;
			return T_semicolon;
		case ':':
			mszTok[count++] = 0;
			return T_colon;
		case ')':
			mszTok[count++] = 0;
			return T_rightparen;
		case '(':
			mszTok[count++] = 0;
			return T_leftparen;
		case ']':
			mszTok[count++] = 0;
			return T_rightbracket;
		case '[':
			mszTok[count++] = 0;
			return T_leftbracket;			
		case '.':
			mszTok[count++] = 0;
			return T_endmarker;
		default :
			fail(1);
			return false;
		}
	}	
	
	/////////////////////////////////////////
	//RE : ..
	//returns the code for the type of operator found 
	//false for not found
	/////////////////////////////////////////
	int IsDoubleDot()
	{
		int state = 0;
		int count = 0;
		char ch = mCS.GetNextChar();
		mszTok[count++] = ch;
		while(1)
		{			
			switch (state)
			{
			case 0:
				if( ch == '.') 
					state = 1;				
				else 
				{
					fail(count);
					return false;
				}
				break;
			case 1:
				ch = mCS.GetNextChar();
				mszTok[count++] = ch;
				if( ch == '.')
					state = 2;
				else 
				{
					fail(count);
					return false;
				}
				break;
			case 2:
				mszTok[count++] = 0;
				return true;				
			}
		}
	}
	
	/////////////////////////////////////////
	//RE : :=
	//returns the code for the type of operator found 
	//false for not found
	/////////////////////////////////////////
	int IsAssignOp()
	{
		int state = 0;
		int count = 0;
		char ch = mCS.GetNextChar();
		mszTok[count++] = ch;
		while(1)
		{			
			switch (state)
			{
			case 0:
				if( ch == ':') 
					state = 1;				
				else 
				{
					fail(count);
					return false;
				}
				break;
			case 1:
				ch = mCS.GetNextChar();
				mszTok[count++] = ch;
				if( ch == '=')
					state = 2;
				else 
				{
					fail(count);
					return false;
				}
				break;
			case 2:
				mszTok[count++] = 0;
				return true;				
			}
		}
	}	
	/////////////////////////////////////////
	//RE : *|/
	//returns the code for the type of operator found 
	//false for not found
	/////////////////////////////////////////
	int IsMulOp()
	{
		int state = 0;
		int count = 0;
		char ch = mCS.GetNextChar();
		mszTok[count++] = ch;
		while(1)
		{			
			switch (state)
			{
			case 0:
				if( ch == '*') 
					state = 1;
				else if( ch == '/')
					state = 2;
				else 
				{
					fail(count);
					return false;
				}
				break;
			case 1:
				mszTok[count++] = 0;
				return OP_mul;
			case 2:
				mszTok[count++] = 0;
				return OP_div;				
			}
		}
	}
	/////////////////////////////////////////
	//RE : +|-
	//returns the code for the type of operator found 
	//false for not found
	//If the type of the last token scanned is
	//RIGHTPAREN, RIGHTBRACKET, IDENTIFIER, or CONSTANT, 
	//then assign the type ADDOP and the appropriate value. 
	//Otherwise, it is a unary operator
	/////////////////////////////////////////
	int IsAddOp()
	{
		int state = 0;
		int count = 0;
		char ch = mCS.GetNextChar();
		mszTok[count++] = ch;
		while(1)
		{			
			switch (state)
			{
			case 0:
				if( ch == '+') 
					state = 1;
				else if( ch == '-')
					state = 2;
				else 
				{
					fail(count);
					return false;
				}
				break;
			case 1:
				mszTok[count++] = 0;
				if( mPreToken.Class == T_rightparen  ||
					mPreToken.Class == T_rightbracket||
					mPreToken.Class == T_identifier  ||
					mPreToken.Class == T_constant    )
				   return OP_plus;
				else
					return T_unaryplus;

				break;
			case 2:
				mszTok[count++] = 0;
				if( mPreToken.Class == T_rightparen  ||
					mPreToken.Class == T_rightbracket||
					mPreToken.Class == T_identifier  ||
					mPreToken.Class == T_constant    )
				   return OP_minus;
				else
					return T_unaryminus;
				break;
			}
		}
	}
	
	/////////////////////////////////////////
	//RE : <(=|>|e) | = | >(=|e) 
	//returns the code for the type of operator found 
	//false for not found
	/////////////////////////////////////////
	int IsRelOp()
	{
		int state = 0;
		int count = 0;
		char ch = mCS.GetNextChar();
		mszTok[count++] = ch;
		while(1)
		{			
			switch (state)
			{
			case 0:
				if( ch == '<') 
					state = 1;
				else if( ch == '=')
					state = 5;
				else if( ch == '>')
					state = 6;
				else
				{
					fail(count);
					return false;
				}
				break;
			case 1:
				ch = mCS.GetNextChar(); mszTok[count++] = ch;

				if( ch == '=')
					state = 2;
				else if( ch == '>')
					state = 3;
				else 
					state = 4;
				break;
			case 2:
				mszTok[count++] = 0;
				return OP_le;
			case 3:
				mszTok[count++] = 0;
				return OP_ne;
			case 4:
				mszTok[--count] = 0;
				mCS.Retreat(1);
				return OP_lt;
			case 5:
				mszTok[count++] = 0;
				return OP_eq;
			case 6:
				ch = mCS.GetNextChar(); mszTok[count++] = ch;
				if( ch == '=')
					state = 7;
				else
					state = 8;
				break;
			case 7:
				mszTok[count++] = 0;
				return OP_ge;
			case 8:
				mszTok[--count] = 0;
				mCS.Retreat(1);
				return OP_gt;
			}
		}
	}
	
	/////////////////////////////////////////
	// RE : [0-9][0-9]*
	/////////////////////////////////////////
	int IsIntegerConst()
	{
		int state = 0;
		int count = 0;
		char ch = mCS.GetNextChar();
		mszTok[count++] = ch;
		while(1)
		{			
			switch (state)
			{
			case 0:
				if( isdigit(ch) )
					state = 1;
				else
				{
					fail(count);
					return false;
				}
				break;
			case 1:
				ch = mCS.GetNextChar(); 
				mszTok[count++] = ch;
				if( count > 5)
				{
					ErrManager::Register(
						Error::Create(ERR_CONST_TOO_LONG,mCS.LineNo(),0)
						);	
					state = 2;
					break;
				}
				if( isdigit(ch) )
					state = 1;
				else
					state = 3;
				break;
			case 2:
				ch = mCS.GetNextChar();
				if( isdigit(ch) )
					state = 2;
				else
					mCS.Retreat();
					mszTok[count] = 0;
					mErrBit = 1;
					return true;
				break;

			case 3:
				if( isdelim(ch) )
				{
					mCS.Retreat();
					mszTok[--count] = 0;					
					return true;
				}
				else
				{
					fail(count);
					return false;
				}
				break;
			}
		}
	}
	
	///////////////////////////////////////////////
	// RE: [0-9][0-9]*.[0-9][0-9]*E(+|-|e)[0-9][0-9]* 
	///////////////////////////////////////////////
	int IsRealConst1()
	{
		int state = 0;
		int count = 0;
		char ch = mCS.GetNextChar();
		mszTok[count++] = ch;
		
		while(1)
		{
			switch (state)
			{
			case 0:
				if( isdigit(ch) )
					state = 1;
				else
				{
					fail(count);
					return false;
				}
				break;
			case 1:
				ch = mCS.GetNextChar(); mszTok[count++] = ch;
				
				if( isdigit(ch) )
					state = 1;
				else if( ch == '.' )
					state = 2;
				else
				{
					fail(count);
					return false;
				}
				break;
			case 2:
				ch = mCS.GetNextChar(); mszTok[count++] = ch;
				if( isdigit(ch) )
					state = 3;
				else
				{
					fail(count);
					return false;
				}
				break;
			case 3:
				ch = mCS.GetNextChar(); mszTok[count++] = ch;
				if( isdigit(ch) )
					state = 3;
				else if( ch == 'E')
					state = 4;
				else
				{
					fail(count);
					return false;
				}
				break;
			case 4:
				ch = mCS.GetNextChar(); mszTok[count++] = ch;
				if( ch == '+' || ch == '-')
					state = 5;
				else if( isdigit(ch) )
				{
					mCS.Retreat(1);
					state = 5;
					count--;
				}
				else
				{
					fail(count);
					return false;
				}
				break;
			case 5:
				ch = mCS.GetNextChar(); mszTok[count++] = ch;
				if( isdigit(ch) )
					state = 6;
				else
				{
					fail(count);
					return false;
				}
				break;
			case 6:
				ch = mCS.GetNextChar(); mszTok[count++] = ch;
				if( isdigit(ch) )
					state = 6;
				else if( isdelim(ch) )
					state = 7;
				else 
				{
					fail(count);
					return false;				
				}
				break;
			case 7:
				mCS.Retreat();
				mszTok[--count] = 0;
				return true;
			}	
			
		}
	}	
	///////////////////////////////////////////////
	// RE: [0-9][0-9]*.[0-9][0-9]*
	///////////////////////////////////////////////
	int IsRealConst2()
	{
		int state = 0;
		int count = 0;
		char ch = mCS.GetNextChar();
		mszTok[count++] = ch;
		
		while(1)
		{
			switch (state)
			{
			case 0:
				if( isdigit(ch) )
					state = 1;
				else
				{
					fail(count);
					return false;
				}
				break;
			case 1:
				ch = mCS.GetNextChar(); mszTok[count++] = ch;
				
				if( isdigit(ch) )
					state = 1;
				else if( ch == '.' )
					state = 2;
				else
				{
					fail(count);
					return false;
				}
				break;
			case 2:
				ch = mCS.GetNextChar(); mszTok[count++] = ch;
				if( isdigit(ch) )
					state = 3;
				else
				{
					fail(count);
					return false;
				}
				break;
			case 3:
				ch = mCS.GetNextChar(); mszTok[count++] = ch;
				if( isdigit(ch) )
					state = 3;
				else if( isdelim(ch) )
					state = 4;
				else
				{
					fail(count);
					return false;
				}
				break;
			case 4:
				mCS.Retreat();
				mszTok[--count] = 0;
				return true;
			}	
			
		}
	}
	/////////////////////////////////////////
	
	/////////////////////////////////////////
	int IsStringConst()
	{
		int state = 0;
		int count = 0;
		char ch = mCS.GetNextChar();
		mszTok[count++] = ch;
		
		while(1)
		{
			switch (state)
			{
			case 0:
				if( ch == '\'' )
					state = 1;
				else
				{
					fail(count);
					return false;
				}
				break;
			case 1:
				ch = mCS.GetNextChar(); mszTok[count++] = ch;
				
				if( ch == '\'')
				{					
					fail(count);
					return false;
				}
				else
					state = 2;
				break;
			case 2:
				ch = mCS.GetNextChar(); mszTok[count++] = ch;

				if( ch == '\'' )
					state = 3;
				else
					state = 2;
				break;
			case 3:
				mszTok[count++] = 0;
				return true;
				break;
			}
		}
	}
	
	/////////////////////////////////////////
	int IsCharConst()
	{
		int state = 0;
		int count = 0;
		char ch = mCS.GetNextChar();
		mszTok[count++] = ch;
		
		while(1)
		{			
			switch (state)
			{
			case 0:
				if( ch == '\'' )
					state = 1;
				else
				{
					fail(count);
					return false;
				}
				break;
			case 1:
				ch = mCS.GetNextChar(); mszTok[count++] = ch;
				if( ch == '\'' )
				{
					ErrManager::Register(
						Error::Create(ERR_EMPTY_CHAR_CONST,
							mCS.LineNo(),mCS.CharPos())
							);

					fail(count);
					return false;
				}
				else
					state = 2;
				break;
			case 2:
				if( ch == '\'' )
					state = 3;
				else
				{
					ErrManager::Register(
						Error::Create(ERR_ILLEGAL_CHAR_CONST,
							mCS.LineNo(),mCS.CharPos())
							);
					fail(count);
					return false;
				}
				break;
			case 3:
				mszTok[count++] = 0;
				return true;
				break;
			}
		}
	}
	/* Look For Identifier */
	int IsIdentifier()
	{
		int state = 0;
		int count = 0;
		char ch = mCS.GetNextChar(); mszTok[count++] = ch ;
		
		while(1)
		{
			switch (state)
			{
			case 0:
				if( isalpha(ch) )
					state = 1;
				else
				{
					fail(count);
					return false;
				}
				break;
			case 1:
				ch = mCS.GetNextChar(); 
				if( count < IDNTIFIER_LEN)
					mszTok[count++] = ch ;
				
				if( isalnum(ch) )
					state = 1;
				else if ( isdelim(ch) )
					state = 2;
				else
				{
					fail(count);
					return false;
				}
				break;
			case 2:
				mCS.Retreat();
				mszTok[--count] = 0;
				return true;
			}
		}
	}
	//////////////Keyword Search/////////////////////////
	int GetKeywordId(char* str)
	{
		for(int i = 0; i < gKeyCount ; i++)
		{
			if( strcmp(gKeyWords[i],str) == 0)
				return gKeyId[i];
		}
		return T_null;
	}
	/////////////////////////////////////////////////////

};

#endif
