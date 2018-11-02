#ifndef PURCHASE_H
#define PURCHASE_H
#include "iobuffer.h"
#include<iostream>
#include <string>

#define STDMAXBUF 256
#define PURCASE_ID_LEN 16
#define LECTURE_ID 12
#define MILEAGE_LEN 10

using namespace std;

class Purchase {
	string purchaseID;
	string lectureID;
	string memberID;
	string Mileage;

	string key;
public:
	Purchase();

	Purchase &operator=(const Purchase &);
	bool operator==(const Purchase &);
	bool operator!=(const Purchase &);

	friend istream & operator >> (istream &is, Purchase &s);
	friend ostream & operator << (ostream &os, Purchase &s);

	bool Unpack(IOBuffer &Buffer);
	bool Pack(IOBuffer &Buffer) const;

	void update_purchaseID(const string new_purchaseID) { purchaseID = new_purchaseID; }
	void update_lectureID(const string new_lectureID) { lectureID = new_lectureID; }
	void update_memberID(const string new_memberID) { memberID = new_memberID; }
	void update_Mileage(const string new_mileage) { Mileage = new_mileage; }

	string get_PID();
	string get_MID();
	string get_LID();

	char *Key();
};

#endif