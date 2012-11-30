#ifndef CACHE_CHAR_STREAM_H
#define CACHE_CHAR_STREAM_H

#include "CharStream.h"

class CacheCharStream 
{
private:
	char* mCache;
	int mIndex;
	int mCacheLen;
	int mCharCount;
	CharStream mCS;
public:
	CacheCharStream(char* fileName ,int cacheLen):mCS(fileName)
	{
		mCacheLen = cacheLen;
		mCharCount = 0;
		mCache = new char[cacheLen + 3];
		mIndex = 0;
	}
	
	char GetNextChar()
	{
		if( mIndex < mCharCount)
			return mCache[mIndex++];
		else
		{			
			char NextC = mCS.GetNextChar();
			if( (mIndex +1) == mCacheLen)
			{
				ClearCache();			
			}
			mCache[mIndex] = NextC;
			mIndex++;
			mCharCount++;
			return NextC;
		}
	}
	void Retreat(int count = 1)
	{
		if( (mIndex - count) > 0)
			mIndex-=count;
		else
			mIndex=0;
	}
	void ClearCache()
	{
		int base = 0;
		for( int i = mIndex ; i < mCharCount ; i++)
		{
			mCache[base++] = mCache[i];
		}
		mIndex = 0 ;
		mCharCount = base;
	}
	int eof ()
	{
		return mCS.eof();
	}
	int LineNo()
	{
		return mCS.LineNo();
	}
	int CharPos()
	{
		return mCS.CharPos();
	}
};

#endif