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
	char f2[] = "fileOfLecture.dat";
	char f3[] = "fileOfPurchase.dat";
	DelimFieldBuffer buf1('|'), buf2('|'), buf3('|');
	RecordFile<Member> MemberFile(buf1);
	RecordFile<Lecture> LectureFile(buf2);
	RecordFile<Purchase> PurchaseFile(buf3);

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

int record_type(string action) {
	bool exit_flag = false;

	string buf;
	int type;
		while (!exit_flag) {
			clear_console();
			cout << "******************************" << endl;
			cout << "* " << action << " Record              *" << endl;
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
	bool action_flag = false;
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
			action_flag = true; record_search(); break;
		case 2:
			action_flag = true; record_insert(); break;
		case 3:
			action_flag = true; record_delete(); break;
		case 4:
			action_flag = true; record_update(); break;
		case 5:
			exit_flag = true; break;
		default:
			cout << endl << "Please Input Correctly" << endl; break;
		}
		if (!action_flag) {
			action_flag = true;
			cout << endl << "If you want to CONTINUE, press Enter..." << endl;
			cin.get();
			clear_console();
		}
	}
}

void record_search() {
	bool exit_flag = false;
	bool type_flag = false;
	vector<int> idx;
	make_memories();
	while (!exit_flag) {
		string search_key;
		int type;
		clear_console();
		type = record_type("Search");
		switch (type) {
		case 1:
			cout << endl << " Input Member ID : ";
			cin >> search_key;
			cin.get();
			if (search_member(search_key)) {
				cout << endl << Members.find(search_key)->second;
			}
			else
				cout << endl << "There's no such Member" << endl;
			break;
		case 2:
			cout << endl << " Input Lecture ID : ";
			cin >> search_key;
			cin.get();
			if (search_lecture(search_key)) {
				cout << endl << Lectures.find(search_key)->second;
			}
			else
				cout << endl << "There's no such Lecture" << endl;
			break;
		case 3:
			cout << endl << " Input Key to search Purchase : ";
			cin >> search_key;
			cin.get();
			idx = search_purchase(search_key);
			if (idx.size() != 0) {
				cout << endl;
				for (auto it = idx.begin(); it != idx.end(); it++) {
					cout << Purchases.at(*it);
				}
			}
			else
				cout << endl << "There's no Suct Purchase" << endl;
			break;
		case 4:
			exit_flag = true; break;
		default:
			cout << endl << "Please Input Correctly" << endl; break;
		}
		cout << endl << "If you want to CONTINUE, press Enter..." << endl;
		cin.get();
		clear_console();
	}
	delete_memories();
}

bool search_member(string key) {
	if (Members.find(key) != Members.end())
		return true;
	else
		return false;
}

bool search_lecture(string key) {
	if (Lectures.find(key) != Lectures.end())
		return true;
	else
		return false;
}

vector<int> search_purchase(string key) {
	vector<int> ret;
	int i = 0;
	for (auto it = Purchases.begin(); it != Purchases.end(); it++,i++) {
		if (!key.compare(it->get_PID()))
			ret.push_back(i);
		else if (!key.compare(it->get_MID()))
			ret.push_back(i);
		else if (!key.compare(it->get_LID()))
			ret.push_back(i);
	}
	return ret;
}

bool search_PID(string key) {
	for (auto it = Purchases.begin(); it != Purchases.end(); it++)
		if (!key.compare(it->get_PID()))
			return true;
	return false;
}

void record_insert() {

}

void record_delete() {

}

void record_update() {

}