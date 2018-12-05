#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<sstream>
#include<fstream>
#include<string>
#include<cstdlib>
#include<ctime>
#include<vector>
#include"shell.h"

#define NUM_MEMBER 1000
#define NUM_LECTURE 1000
#define NUM_PURCHASE 100
#define MAXBUF 256
using namespace std;

#ifdef DATA
int main() {
	char member[] = "listOfMember.txt";
	char lecture[] = "listOfLecture.txt";
	char purchase[] = "listOfPurchase.txt";
	char buffer[MAXBUF];
	char subjects[20][20] = { "LinearAlgebra","Calculus","DigitalCircuit","History","Network","SetTheory","Algorithm","OS","Assembly",
							"English","Architecture","PL","FP","ECA","Automata",
							"Physics","Statistics","Chemistry","Economics","Literature" };
	vector<string> MEMBERS;
	vector<string> LECTURES;
	srand(time(NULL));
	// member write
	ofstream fp;
	fp.open(member);
	fp << NUM_MEMBER << endl;
	fp << "TestUser|T1234|Test|010-1234-1234|Seoul|0000010000|9" << endl;
	fp << "admin|adminpass|admin|010-1111-1111|Seoul|9999999999|1" << endl;
	MEMBERS.push_back(string("admin"));
	MEMBERS.push_back(string("TestUser"));
	for (int i = 2; i < NUM_MEMBER; i++) {
		string ID, PW, Name, PH, Address,Mileage;
		for (int j = 0; j < 8; j++) {
			PW += rand() % 10 + '0';
		}
		ID = "MID_00" + to_string(i);
		MEMBERS.push_back(ID);
		Name = "MNAME_" + to_string(i);
		sprintf(buffer, "010-%04d-%04d", rand() % 10000, rand() % 10000);
		PH = buffer;
		Address = "Seoul";
		for (int j = 0; j < 10; j++)
			Mileage += rand() % 10 + '0';
		fp << ID << "|" << PW << "|" << Name << "|" << PH << "|" << Address << "|" << Mileage << "|" << "9" << endl;
	}
	fp.close();
	// lecture write
	fp.open(lecture);
	fp << NUM_LECTURE << endl;
	for (int i = 0; i < NUM_LECTURE; i++) {
		string ID, Subject, Teacher, Textbook;
		for (int j = 0; j < 12 - to_string(i).size(); j++)
			ID += '0';
		ID += to_string(i);
		while (ID.size() < 12)
			ID += "0";
		LECTURES.push_back(ID);
		Subject = subjects[rand() % 20];
		char level = rand() % 4 + 'A';
		string Price;
		Price += rand() % 9 + '1'; Price += rand() % 10 + '0';
		Price += "0,000";
		char extension;
		if (rand() % 2) extension = 'Y'; else extension = 'N';
		int duedate;
		if (rand() % 3 == 0) duedate = 1; else if (rand() % 2) duedate = 2; else duedate = 3;
		Teacher = "TNAME_" + to_string(i);
		Textbook = Subject + " Book";
		fp << ID << "|" << Subject << "|" << level << "|" << Price << "|" << extension << "|" << duedate*30 << "|" << Teacher << "|" << Textbook << endl;
	}
	fp.close();
	// purchase write
	fp.open(purchase);
	fp << NUM_PURCHASE << endl;
	// use 21~120 for purchase ID
	bool check[100];
	int pin = 28;
	for (int i = 0; i < 100; i++) check[i] = false;
	for (int i = 0; i < NUM_PURCHASE; i++) {
		int a = rand() %  NUM_MEMBER;
		string PID, LID, MID,Mileage;
		while (a == 0) a = rand() % NUM_MEMBER;
		MID = MEMBERS.at(a);
		if (i == 0 || i == 1) MID = "TestUser";
		LID = LECTURES.at(rand() % NUM_LECTURE);
		int n = rand() % 100;
		while (check[n]) n = rand() % 100;
		check[n] = true;
		char tmp = pin + n;
		if (tmp == '|') tmp = 27;
		PID += tmp;// static_cast<char> (pin + n);
		/*for (int j = 0; j < 8; j++)
			PID += "0";
		PID += to_string(i);
		for (int j = PID.size(); j < 16; j++)
			PID += "0";
		*/
		Mileage = "000000";
		Mileage += rand() % 9 + '1';
		Mileage += "000";
		fp << PID << "|" << LID << "|" << MID << "|" << Mileage << endl;
	}
	fp.close();

	return 0;
}

#endif