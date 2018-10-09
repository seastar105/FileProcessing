#ifndef MEMBER_H
#define MEMBER_H
#include<iostream>
#include"iobuffer.h"
#include <string>

#define STDMAXBUF 256
#define MILEAGE_LEN 10

using namespace std;

class Member {
	string ID;
	string Password;
	string Name;
	string PhoneNumber;
	string Address;
	string Mileage;
public:
	Member();

	Member(const string, const string, const string, const string, const string, const string);

	Member &operator=(const Member &);
	bool operator==(const Member &);
	bool operator!=(const Member &);

	friend istream & operator >> (istream &is, Member &s);
	friend ostream & operator << (ostream &os, Member &s);

	bool Unpack(IOBuffer &Buffer);
	bool Pack(IOBuffer &Buffer) const;

	void update_ID(const string new_id) { ID = new_id; }
	void update_Password(const string new_password) { Password = new_password; }
	void update_Name(const string new_name) { Name = new_name; }
	void update_PhoneNumber(const string new_phonenumber) { PhoneNumber = new_phonenumber; }
	void update_Address(const string new_address) { Address = new_address; }
	void update_Mileage(const string new_mileage) { this->Mileage = new_mileage; }

	string get_ID();
};

#endif