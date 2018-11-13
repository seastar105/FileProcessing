
//textind.cc
#include "textind.h"
#include <iostream>
#include <string>

using namespace std;

int TextIndex::Update(const char* key, int dst) {		// need to modify all the recaddrs if needed
	int recaddr = RecAddrs[Find(key)];
	for (int i = 0; i < NumKeys; i++) {
		if (RecAddrs[i] > recaddr) {
			RecAddrs[i] += dst;
		}
	}
	return 1;
}

int TextIndex::FirstRecAddr() {
	if (NumKeys == 0)
		return -1;
	else {
		cur = 0;
		return RecAddrs[cur];
	}
}

int TextIndex::NextRecAddr() {
	if (++cur >= NumKeys) {			// end of index
		//cur = -1;
		return -1;
	}
	return RecAddrs[cur];
}

TextIndex::TextIndex(int maxKeys, int unique)
	: NumKeys(0), Keys(0), RecAddrs(0), VecAddrs(0),Lengths(0)
{
	Init(maxKeys, unique);
}

TextIndex :: ~TextIndex()
{
	delete Keys; delete RecAddrs; delete VecAddrs; delete Lengths;
}

int TextIndex::Insert(const char * key, int recAddr)
{
	int i;
	int index = Find(key);
	if (Unique && index >= 0) return 0; // key already in
	if (NumKeys == MaxKeys) return 0; //no room for another key
	for (i = NumKeys - 1; i >= 0; i--)
	{
		if (strcmp(key, Keys[i]) > 0) break; // insert into location i+1
		Keys[i + 1] = Keys[i];
		RecAddrs[i + 1] = RecAddrs[i];
		VecAddrs[i + 1] = VecAddrs[i];
	}
	Keys[i + 1] = _strdup(key);
	RecAddrs[i + 1] = recAddr;
	VecAddrs[i + 1] = NumKeys;
	NumKeys++;
	return 1;
}

int TextIndex::Remove(const char * key)
{
	int index = Find(key);
	if (index < 0) return 0; // key not in index
	for (int i = index; i < NumKeys; i++)
	{
		Keys[i] = Keys[i + 1];
		RecAddrs[i] = RecAddrs[i + 1];
	}
	NumKeys--;
	return 1;
}

int TextIndex::Remove(const char * key, size_t len)
{
	int index = Find(key);
	if (index < 0) return 0; // key not in index
	int tmp = VecAddrs[index];
	int pin = RecAddrs[index];
	for (int i = index; i < NumKeys; i++)
	{
		Keys[i] = Keys[i + 1];
		RecAddrs[i] = RecAddrs[i + 1];
	}
	for (int i = 0; i < NumKeys - 1; i++) {
		if (i == index) {
			i++; continue;
		}
		if (VecAddrs[i] > tmp)
			VecAddrs[i]--;
		if (RecAddrs[i] > pin) {
			RecAddrs[i] -= len;
		}
	}
	NumKeys--;
	return 1;
}

int TextIndex::VecIndex(const char *key) const {
	int index = Find(key);
	if (index < 0) return index;
	return VecAddrs[index];
}

int TextIndex::Search(const char * key) const
{
	int index = Find(key);
	if (index < 0) return index;
	return RecAddrs[index];
}

void TextIndex::Print(ostream & stream) const
{
	stream << "Text Index max keys " << MaxKeys
		<< " num keys " << NumKeys << endl;
	for (int i = 0; i < NumKeys; i++)
		stream << "\tKey[" << i << "] " << Keys[i]
		<< " RecAddr " << RecAddrs[i] << endl;
}



int TextIndex::Find(const char * key) const
{
	int low, mid, high;
	int flag = 0;
	low = 0;
	high = NumKeys - 1;
	while (low <= high) {
		int cmp;
		mid = (low + high) / 2;
		cmp = strcmp(key, Keys[mid]);
		if (cmp == 0) {
			flag = 1;
			break;
		}
		else if (cmp > 0)
			low = mid + 1;
		else
			high = mid - 1;
	}
	/*for (int i = 0; i < NumKeys; i++)
		if (strcmp(Keys[i], key) == 0) return i;// key found
//		else if (strcmp(Keys[i], key)>0) return -1;// not found*/
	if(!flag)
		return -1;// not found
	return mid;
}

int TextIndex::Init(int maxKeys, int unique)
{
	Unique = unique != 0;
	if (maxKeys <= 0)
	{
		MaxKeys = 0;
		return 0;
	}
	MaxKeys = maxKeys;
	Keys = new char *[maxKeys];
	RecAddrs = new int[maxKeys];
	VecAddrs = new int[maxKeys];
	Lengths = new int[maxKeys];
	return 1;
}

