#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "stdio.h"
#include "STEntry.h"

static const MAX_SIZE = 10;

class SymbolTable
{
private:
	char STName[50];
	int mSize;
	STEntry* mEntries[MAX_SIZE];
public:
	SymbolTable()
	{
		mSize = 0;
	}
	int Add(STEntry* entry)
	{
		if( mSize < MAX_SIZE)
			mEntries[mSize++] = entry;
	}
	// Returns the index of entry if found 
	// Else return -1
		
	int LookUp(STEntry* entry)
	{
		for( int i = 0 ; i < mSize ; i++)
		{
			if( entry == mEntries[i])
				return i;
			else
				return -1;
		}
	}
	void Dump()
	{
		printf("\t%s\t",STName);
		for( int i = 0 ; i < mSize ; i++)
			mEntries[i]->Dump();
		printf("\n",STName);
	}
	~SymbolTable()
	{
		printf("Destructing:\t%s\t",STName);
		for( int i = 0 ; i < mSize ; i++)
			delete mEntries[i];		
	}
};
#endif