#include "Lecture.h"
#include <sstream>

Lecture::Lecture() {
}

Lecture& Lecture::operator=(const Lecture &L) {
	update_ID(L.ID);
	update_Subject(L.Subject);
	update_level(L.level);
	update_Price(L.Price);
	update_Extension(L.Extension);
	update_dueDate(L.dueDate);
	update_nameOfTeacher(L.nameOfTeacher);
	update_Textbook(L.Textbook);

	return *this;
}

bool Lecture::operator==(const Lecture &L) {
	if (!ID.compare(L.ID))
		return true;
	else
		return false;
}

bool Lecture::operator!=(const Lecture &L) {
	if (ID.compare(L.ID))
		return true;
	else
		return false;
}

istream & operator >> (istream &is, Lecture & L) {
	char *buffer = new  char[STDMAXBUF];
	char *ptr, *context = NULL;
	is.getline(buffer, STDMAXBUF);
	ptr = strtok_s(buffer, "|", &context);
	L.update_ID(ptr);
	ptr = strtok_s(NULL, "|", &context);
	L.update_Subject(string(ptr));
	ptr = strtok_s(NULL, "|", &context);
	L.update_level(ptr[0]);
	ptr = strtok_s(NULL, "|", &context);
	L.update_Price(commaDecimalToInt(string(ptr)));
	ptr = strtok_s(NULL, "|", &context);
	L.update_Extension(ptr[0]);
	ptr = strtok_s(NULL, "|", &context);
	L.update_dueDate(commaDecimalToInt(string(ptr)));
	ptr = strtok_s(NULL, "|", &context);
	L.update_nameOfTeacher(string(ptr));
	ptr = strtok_s(NULL, "|", &context);
	L.update_Textbook(string(ptr));
	ptr = strtok_s(NULL, "|", &context);
	delete[] buffer;
	if (ptr) cout << "error" << endl;

	return is;
}

ostream & operator << (ostream &os, Lecture &L) {
	char sep = ', ';
	os << L.ID << sep << L.Subject << sep << L.level << sep << L.format_price() << sep << L.Extension << sep << 
		L.dueDate << sep << L.nameOfTeacher << sep << L.Textbook << endl;

	return os;
}

int commaDecimalToInt(string str) {
	int mul = 1, ret = 0;
	for (int i = str.size() - 1; i >= 0; i--) {
		if (str[i] == ',') continue;
		ret += mul * (str[i] - '0');
		mul *= 10;
	}

	return ret;
}

string Lecture::format_price() {
	return to_string(Price / 1000) + ",000";
}

bool Lecture::Unpack(IOBuffer &Buffer) {
	int numBytes;
	char buf[STDMAXBUF];

	numBytes = Buffer.Unpack(buf, STDMAXBUF);
	if (numBytes == -1) return false;
	ID = string(buf,numBytes);

	numBytes = Buffer.Unpack(buf, STDMAXBUF);
	if (numBytes == -1) return false;
	Subject = string(buf, numBytes);

	numBytes = Buffer.Unpack(buf, STDMAXBUF);
	if (numBytes == -1) return false;
	level = buf[0];

	numBytes = Buffer.Unpack(buf, STDMAXBUF);
	if (numBytes == -1) return false;
	Price = commaDecimalToInt(string(buf, numBytes));

	numBytes = Buffer.Unpack(buf, STDMAXBUF);
	if (numBytes == -1) return false;
	Extension = buf[0];

	numBytes = Buffer.Unpack(buf, STDMAXBUF);
	if (numBytes == -1) return false;
	dueDate = commaDecimalToInt(string(buf, numBytes));

	numBytes = Buffer.Unpack(buf, STDMAXBUF);
	if (numBytes == -1) return false;
	nameOfTeacher = string(buf, numBytes);

	numBytes = Buffer.Unpack(buf, STDMAXBUF);
	if (numBytes == -1) return false;
	Textbook = string(buf, numBytes);

	return true;
}

bool Lecture::Pack(IOBuffer &Buffer) const {
	int numBytes;

	Buffer.Clear();

	string s_level(1, level);
	string s_extension(1, Extension);
	string s_price = to_string(Price / 1000) + ",000";
	string s_dueDate = to_string(dueDate);

	numBytes = Buffer.Pack(ID.c_str());
	if (numBytes == -1) return false;

	numBytes = Buffer.Pack(Subject.c_str());
	if (numBytes == -1) return false;

	numBytes = Buffer.Pack(s_level.c_str());
	if (numBytes == -1) return false;

	numBytes = Buffer.Pack(s_price.c_str());
	if (numBytes == -1) return false;

	numBytes = Buffer.Pack(s_extension.c_str());
	if (numBytes == -1) return false;

	numBytes = Buffer.Pack(s_dueDate.c_str());
	if (numBytes == -1) return false;

	numBytes = Buffer.Pack(nameOfTeacher.c_str());
	if (numBytes == -1) return false;

	numBytes = Buffer.Pack(Textbook.c_str());
	if (numBytes == -1) return false;

	return true;
}

string Lecture::get_ID() {
	return ID;
}

char * Lecture::Key() {
	key.assign(ID);
	return const_cast<char*>(key.c_str());
}