#include "Member.h"
#include <sstream>

Member::Member() {
	
}

Member::Member(const string ID, const string Password, const string Name, const string PhoneNumber, const string Address,const string Mileage) {
	this->ID = ID;
	this->Password = Password;
	this->Name = Name;
	this->PhoneNumber = PhoneNumber;
	this->Address = Address;
	this->Mileage = Mileage;
}

Member& Member::operator=(const Member &m) {
	update_ID(m.ID);
	update_Password(m.Password);
	update_Name(m.Name);
	update_PhoneNumber(m.PhoneNumber);
	update_Address(m.Address);
	update_Mileage(m.Mileage);

	return *this;
}

bool Member::operator==(const Member &m) {
	if (!ID.compare(m.ID))
		return true;
	else
		return false;
}

bool Member::operator!=(const Member &m) {
	if (ID.compare(m.ID))
		return true;
	else
		return false;
}

istream & operator >> (istream &is, Member &s) {
	char *buffer = new  char[STDMAXBUF];
	char *ptr, *context = NULL;
	is.getline(buffer, STDMAXBUF);
	ptr = strtok_s(buffer, "|", &context);
	s.ID = string(ptr);
	ptr = strtok_s(NULL, "|", &context);
	s.Password = string(ptr);
	ptr = strtok_s(NULL, "|", &context);
	s.Name = string(ptr);
	ptr = strtok_s(NULL, "|", &context);
	s.PhoneNumber = string(ptr);
	ptr = strtok_s(NULL, "|", &context);
	s.Address = string(ptr);
	ptr = strtok_s(NULL, "|", &context);
	s.Mileage = string(ptr);
	ptr = strtok_s(NULL, "|", &context);
	delete[] buffer;
	if (ptr) cout << "error" << endl;

	return is;
}

ostream & operator << (ostream &os, Member &s) {
	char sep = ', ';
	os << s.ID << sep << s.Password << sep << s.Name << sep << s.PhoneNumber << sep << s.Address << sep << s.Mileage << endl;
	return os;
}

bool Member::Unpack(IOBuffer &Buffer) {
	// assume there's always delimiter
	int numBytes;
	char buf[STDMAXBUF];
	numBytes = Buffer.Unpack(buf, STDMAXBUF);
	if (numBytes == -1) return false;
	this->ID = string(buf, numBytes);
	numBytes = Buffer.Unpack(buf, STDMAXBUF);
	if (numBytes == -1) return false;
	this->Password = string(buf, numBytes);
	numBytes = Buffer.Unpack(buf, STDMAXBUF);
	if (numBytes == -1) return false;
	this->Name = string(buf, numBytes);
	numBytes = Buffer.Unpack(buf, STDMAXBUF);
	if (numBytes == -1) return false;
	this->PhoneNumber = string(buf, numBytes);
	numBytes = Buffer.Unpack(buf, STDMAXBUF);
	if (numBytes == -1) return false;
	this->Address = string(buf, numBytes);
	numBytes = Buffer.Unpack(buf, STDMAXBUF);
	if (numBytes == -1) return false;
	this->Mileage = string(buf, numBytes);

	return true;
}

bool Member::Pack(IOBuffer & Buffer) const {
	int numBytes;

	Buffer.Clear();

	numBytes = Buffer.Pack(ID.c_str());
	if (numBytes == -1) return false;
	numBytes = Buffer.Pack(Password.c_str());
	if (numBytes == -1) return false;
	numBytes = Buffer.Pack(Name.c_str());
	if (numBytes == -1) return false;
	numBytes = Buffer.Pack(PhoneNumber.c_str());
	if (numBytes == -1) return false;
	numBytes = Buffer.Pack(Address.c_str());
	if (numBytes == -1) return false;
	numBytes = Buffer.Pack(Mileage.c_str());
	if (numBytes == -1) return false;

	return true;
}

string Member::get_ID() {
	return ID;
}