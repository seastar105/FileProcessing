#ifndef LECTURE_H
#define LECTURE_H
#include<iostream>
#include"iobuffer.h"
#include <string>

#define STDMAXBUF 256
#define LECTURE_ID_LEN 12

using namespace std;

int commaDecimalToInt(string);

class Lecture {
	string ID;
	string Subject;
	char level;
	int Price;
	char Extension;
	int dueDate;
	string nameOfTeacher;
	string Textbook;

	string key;
public:
	Lecture();

	Lecture &operator=(const Lecture &);
	bool operator==(const Lecture &);
	bool operator!=(const Lecture &);

	friend istream & operator >> (istream &is, Lecture &s);
	friend ostream & operator << (ostream &os, Lecture &s);

	bool Unpack(IOBuffer &Buffer);
	bool Pack(IOBuffer &Buffer) const;

	void update_ID(const string new_id) { this->ID = new_id; }
	void update_Subject(const string new_subject) { Subject = new_subject; }
	void update_level(const char new_level) { level = new_level; }
	void update_Price(const int new_price) { Price = new_price; }
	void update_Extension(const char new_extension) { Extension = new_extension; }
	void update_dueDate(const int new_duedate) { dueDate = new_duedate; }
	void update_nameOfTeacher(const string new_nameofteacher) { nameOfTeacher = new_nameofteacher; }
	void update_Textbook(const string new_textbook) { Textbook = new_textbook; }

	string format_price();
	string get_ID();

	char *Key();
};

#endif