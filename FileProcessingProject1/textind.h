#pragma once
//textind.h
#ifndef TEXTIND_H
#define TEXTIND_H
#include <iostream>

using namespace std;

class TextIndex
{
private:
	int cur;
public:
	TextIndex(int maxKeys = 1000, int unique = 1);
	~TextIndex();
	int Update(const char *key, int dst);
	int Insert(const char * key, int recAddr);
	int Remove(const char * key);
	int Remove(const char * key, size_t len);
	int Search(const char * key) const;
	void Print(ostream &) const;
	int FirstRecAddr();
	int NextRecAddr();
	int VecIndex(const char *key) const;
	int * VecAddrs;
	int * Lengths;
	int Find(const char * key) const;
protected:
	int MaxKeys;
	int NumKeys;
	char * * Keys;
	int * RecAddrs;
	int Init(int maxKeys, int unique);
	int Unique; // if true, each key value must be unique in the index
	friend class TextIndexBuffer;
};

#endif

