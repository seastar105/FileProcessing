#include"RecordManipulation.h"

map<string, Member> Members;
map<string, Lecture> Lectures;
vector<Purchase> Purchases;

bool member_mod = false;
bool lecture_mod = false;
bool purchase_mod = false;

using namespace std;

void make_memories() {
	char f1[] = "fileOfMember.dat";
	char f2[] = "fileOfLecutre.dat";
	char f3[] = "fileOfPurchase.dat";
	DelimFieldBuffer buf1('|'), buf2('|'), buf3('|');
	RecordFile<Member> MemberFile(buf1);
	RecordFile<Purchase> PurchaseFile(buf2);
	RecordFile<Lecture> LectureFile(buf3);

	Member M;
	MemberFile.Open(f1, ios::in);
	while (MemberFile.Read(M) != -1) {
		Members[M.get_ID()] = M;
	}
	MemberFile.Close();

	Lecture L;
	LectureFile.Open(f2, ios::in);
	while (LectureFile.Read(L) != -1) {
		Lectures[L.get_ID()] = L;
	}
	LectureFile.Close();

	Purchase P;
	PurchaseFile.Open(f3, ios::in);
	while (PurchaseFile.Read(P) != -1) {
		Purchases.push_back(P);
	}
	PurchaseFile.Close();
}

void delete_memories() {
	if (member_mod) {
		DelimFieldBuffer buffer('|');
		RecordFile<Member> MemberFile(buffer);
		char filename[] = "fileOfMember.dat";
		MemberFile.Open(filename, ios::out | ios::trunc);
		for (auto it = Members.begin(); it != Members.end(); it++) {
			if (MemberFile.Write(it->second) == -1)
				cout << "Write error!" << endl;
		}
		MemberFile.Close();
	}
	if (lecture_mod) {
		DelimFieldBuffer buffer('|');
		RecordFile<Lecture> LectureFile(buffer);
		char filename[] = "fileOfLecture.dat";
		LectureFile.Open(filename, ios::out | ios::trunc);
		for (auto it = Lectures.begin(); it != Lectures.end(); it++) {
			if (LectureFile.Write(it->second) == -1)
				cout << "Write error!" << endl;
		}
		LectureFile.Close();
	}
	if (purchase_mod) {
		DelimFieldBuffer buffer('|');
		RecordFile<Purchase> PurchaseFile(buffer);
		char filename[] = "fileOfPurchase.dat";
		PurchaseFile.Open(filename, ios::out | ios::trunc);
		for (auto it = Purchases.begin(); it != Purchases.end(); it++) {
			if (PurchaseFile.Write(*it) == -1)
				cout << "Write error!" << endl;
		}
		PurchaseFile.Close();
	}
	Members.clear();
	Lectures.clear();
	Purchases.clear();
}

void clear_console() {
	system("cls");
}

int record_type() {
	bool exit_flag = false;

	string buf;
	int type;
		while (!exit_flag) {
			clear_console();
			cout << "******************************" << endl;
			cout << "* Choose Record Type         *" << endl;
			cout << "******************************" << endl;
			cout << "* 1. Member                  *" << endl;
			cout << "* 2. Lecture                 *" << endl;
			cout << "* 3. Purchase                *" << endl;
			cout << "* 4. Exit                    *" << endl;
			cout << "******************************" << endl;
			cout << endl << "Please Input Number : ";
			cin >> buf;
			cin.get();
			if (buf.size() == 1) type = buf.at(0) - '0';
			else
				type = -1;
			if (type == -1) {
				cout << endl << "Please input correctly" << endl;
				clear_console();
				continue;
			}
			else {
				clear_console();
				return type;
			}
		}
}

void purchase_menu() {
	cout << "******************************" << endl;
	cout << "* PurchaseLectureSystem      *" << endl;
	cout << "******************************" << endl;
	cout << "* 1. Search                  *" << endl;
	cout << "* 2. Insert                  *" << endl;
	cout << "* 3. Delete                  *" << endl;
	cout << "* 4. Update                  *" << endl;
	cout << "* 5. Exit                    *" << endl;
	cout << "******************************" << endl;
}

void LecturePurchaseSystem() {
	bool exit_flag=false;
	while (!exit_flag) {
		string buf;
		int selection;
		clear_console();
		purchase_menu();
		cout << endl << "Input : ";
		cin >> buf;
		cin.get();
		if (buf.size() == 1) selection = buf.at(0) - '0';
		else selection = -1;
		switch (selection) {									// 1:search, 2:insert, 3:delete, 4:update,5:exit
		case 1:
			record_search(); break;
		case 2:
			record_insert(); break;
		case 3:
			record_delete(); break;
		case 4:
			record_update(); break;
		case 5:
			exit_flag = true; break;
		default:
			cout << endl << "Please Input Correctly" << endl; break;
		}
		cout << endl << "If you want to CONTINUE, press Enter..." << endl;
		cin.get();
		clear_console();
	}
}

void record_search() {

}

void record_insert() {

}

void record_delete() {

}

void record_update() {

}