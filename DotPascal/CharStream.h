#ifndef _CHARSTREAM_H
#define _CHARSTREAM_H

const char EOF_CHAR = '@';
const char L_CURLY  = '{';
const char R_CURLY  = '}';
const char BLANK    = ' ';
const char TAB      = '\t';
const char EOL_CHAR = '\0';
const char NL_CHAR = '\n';
const char VALID_CHARS[] = "abcdefghijklmnopqrstuvwzyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890.,;:<>/*[]+-=(){\\t' ";

class CharStream
{
private:
	char mCurrLine[256];
	
	int mLineNo;
	int mCurrChar;
	bool mInString;
	FILE* mfp;
	
	void SkipWhite();
	void Advance();
	void SkipComment();
	void GetNewLine();
	bool Valid (char c);
	char CurrChar();
public:
	CharStream(char* filename );
	int eof()
	{
		return feof(mfp);
	}
	int LineNo()
	{
		return mLineNo;
	}
	int CharPos()
	{
		return mCurrChar;
	}
	char GetNextChar();		
};

#endif
