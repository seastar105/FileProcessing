#include "Purchase.h"
#include <sstream>

Purchase::Purchase() {

}

Purchase& Purchase::operator=(const Purchase &p) {
	update_purchaseID(p.purchaseID);
	update_lectureID(p.lectureID);
	update_memberID(p.memberID);
	update_Mileage(p.Mileage);

	return *this;
}

bool Purchase::operator==(const Purchase &p) {
	if (!purchaseID.compare(p.purchaseID))
		return true;
	else
		return false;
}

bool Purchase::operator!=(const Purchase &p) {
	if (purchaseID.compare(p.purchaseID))
		return true;
	else
		return false;
}

istream & operator >> (istream &is, Purchase &p) {
	char *buffer = new  char[STDMAXBUF];
	char *ptr, *context = NULL;
	is.getline(buffer, STDMAXBUF);
	ptr = strtok_s(buffer, "|", &context);
	p.update_purchaseID(string(ptr));
	ptr = strtok_s(NULL, "|", &context);
	p.update_lectureID(string(ptr));
	ptr = strtok_s(NULL, "|", &context);
	p.update_memberID(string(ptr));
	ptr = strtok_s(NULL, "|", &context);
	p.update_Mileage(string(ptr));
	ptr = strtok_s(NULL, "|", &context);
	delete[] buffer;
	if (ptr) cout << "error" << endl;

	return is;
}

ostream & operator << (ostream &os, Purchase &p) {
	char sep = ', ';
	os << p.purchaseID << sep << p.lectureID << sep << p.memberID << sep << p.Mileage << endl;

	return os;
}

bool Purchase::Unpack(IOBuffer &Buffer) {
	int numBytes;
	char buf[STDMAXBUF];

	numBytes = Buffer.Unpack(buf, STDMAXBUF);
	if (numBytes == -1) return false;
	purchaseID = string(buf, numBytes);

	numBytes = Buffer.Unpack(buf, STDMAXBUF);
	if (numBytes == -1) return false;
	lectureID = string(buf, numBytes);

	numBytes = Buffer.Unpack(buf, STDMAXBUF);
	if (numBytes == -1) return false;
	memberID = string(buf, numBytes);

	numBytes = Buffer.Unpack(buf, STDMAXBUF);
	if (numBytes == -1) return false;
	Mileage = string(buf, numBytes);

	return true;
}

bool Purchase::Pack(IOBuffer &Buffer) const {
	int numBytes;

	Buffer.Clear();

	numBytes = Buffer.Pack(purchaseID.c_str());
	if (numBytes == -1) return false;

	numBytes = Buffer.Pack(lectureID.c_str());
	if (numBytes == -1) return false;

	numBytes = Buffer.Pack(memberID.c_str());
	if (numBytes == -1) return false;

	numBytes = Buffer.Pack(Mileage.c_str());
	if (numBytes == -1) return false;

	return true;
}

string Purchase::get_PID() {
	return purchaseID;
}

string Purchase::get_LID() {
	return lectureID;
}

string Purchase::get_MID() {
	return memberID;
}