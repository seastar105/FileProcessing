#include"RecordManipulation.h"
DelimFieldBuffer Membuf('|');
DelimFieldBuffer Lecbuf('|');
DelimFieldBuffer Purbuf('|');
RecordFile<Purchase> PurchaseFile(Purbuf);
BTree<char> Purchase_Index(4);
TextIndexedFile<Member> Member_Index(Membuf, 20, 1200);
TextIndexedFile<Lecture> Lecture_Index(Lecbuf, 20, 1200);
vector<Member> &Members = Member_Index.Records;
vector<Lecture> &Lectures = Lecture_Index.Records;
vector<Purchase> Purchases;

// 필요한 가정 maxKeys가 1000이라고 가정한다.
bool member_mod = false;
bool lecture_mod = false;
bool purchase_mod = false;
bool memidx_exist = false;
bool lecidx_exist = false;
bool puridx_exist = false;
using namespace std;

void openIndex() {			// If There's no ind file, create then open.
	// fileOfMember
	string filename = "fileOfMember";
	int result;
	result = Member_Index.Open(const_cast<char*>(filename.c_str()));
	if (result == 0) {			// 
		DelimFieldBuffer buf('|', 256);
		RecordFile<Member> MemberFile(buf);
		TextIndex MemberIndex(1200);
		MemberFile.Open(const_cast<char*>("fileOfMember.dat"), ios::in);
		while (1) {
			Member m;
			int recaddr = MemberFile.Read(m);
			if (recaddr == -1) break;
			MemberIndex.Insert(m.Key(), recaddr);
		}
		MemberFile.Close();
		TextIndexBuffer idxbuf(20, 1200);
		BufferFile MemIdxFile(idxbuf);
		MemIdxFile.Create(const_cast<char*>("fileOfMember.ind"), ios::out | ios::trunc);
		MemIdxFile.Rewind();
		idxbuf.Pack(MemberIndex);
		MemIdxFile.Write();
		MemIdxFile.Close();
		memidx_exist = true;
		Member_Index.Open(const_cast<char*>(filename.c_str()));
	}
	Members = Member_Index.Records;
	// fileOfLecture
	filename = "fileOfLecture";
	result = Lecture_Index.Open(const_cast<char*>(filename.c_str()));
	if (result == 0) {
		DelimFieldBuffer buf('|', 256);
		RecordFile<Lecture> LectureFile(buf);
		TextIndex LectureIndex(1200);
		LectureFile.Open(const_cast<char*>("fileOfLecture.dat"), ios::in);
		while (1) {
			Lecture m;
			int recaddr = LectureFile.Read(m);
			if (recaddr == -1) break;
			LectureIndex.Insert(m.Key(), recaddr);
		}
		LectureFile.Close();
		TextIndexBuffer idxbuf(20, 1200);
		BufferFile LecIdxFile(idxbuf);
		LecIdxFile.Create(const_cast<char*>("fileOfLecture.ind"), ios::out | ios::trunc);
		LecIdxFile.Rewind();
		idxbuf.Pack(LectureIndex);
		LecIdxFile.Write();
		LecIdxFile.Close();
		lecidx_exist = true;
		Lecture_Index.Open(const_cast<char*>(filename.c_str()));
	}
	Lectures = Lecture_Index.Records;
	// fileOfPurchase
	filename = "fileOfPurchase.ind";
	result = Purchase_Index.Open(const_cast<char*>("fileOfPurchase.ind"), ios::out | ios::in);
	if (result == 0) {										// index file does not exist
		PurchaseFile.Open(const_cast<char*>("fileOfPurchase.dat"), ios::in);
		Purchase_Index.Create(const_cast<char*>("fileOfPurchase.ind"), ios::in | ios::out | ios::trunc);
		while (1) {
			Purchase P;
			int recaddr = PurchaseFile.Read(P);
			if (recaddr == -1) break;
			Purchase_Index.Insert(P.Key()[0], recaddr);
		}
		PurchaseFile.Close();
		Purchase_Index.Close();
		Purchase_Index.Open(const_cast<char*>("fileOfPurchase.ind"), ios::out | ios::in);
		puridx_exist = true;
	}
}

void admin_menu() {
	LecturePurchaseSystem();
}

void general_system() {
	cout << "**************************************" << endl;
	cout << "* PurchaseLectureSystem - General    *" << endl;
	cout << "**************************************" << endl;
	cout << "* 1. Member Information              *" << endl;
	cout << "* 2. Search Lecture                  *" << endl;
	cout << "* 3. Purchase Manipulation           *" << endl;
	cout << "* 4. Exit                            *" << endl;
	cout << "**************************************" << endl;
}

bool member_information(Member &m) {
	string buf;
	int selection;
	clear_console();
	cout << "**************************************" << endl;
	cout << "* 1. Modify my information           *" << endl;
	cout << "* 2. Delete my information           *" << endl;
	cout << "* 3. Return                          *" << endl;
	cout << "**************************************" << endl;
	cout << endl << "Input : ";
	cin >> buf; cin.get();
	if (buf.size() == 1) selection = buf.at(0) - '0';
	else selection = -1;
	switch (selection) {
	case 1:
		if (update_member(m.get_ID())) {
			cout << "Update Success, Press Enter....";
		}
		else {
			cout << endl << "Update Failed, Press Enter...";
		}
		cin.get();
		return true;
	case 2:
		delete_member(m.get_ID());
		cout << endl << "Press Enter...";
		cin.get();
		return false;
	case 3:
		return true;
	}
}

void search_lecture() {
	string search_key;
	cout << endl << " Input Lecture ID : ";
	cin >> search_key;
	cin.get();
	if (search_lecture(search_key)) {
		cout << endl << *(find_if(Lectures.begin(), Lectures.end(), [search_key](Lecture &m) {
			return !search_key.compare(m.Key());
		}));
	}
	else
		cout << endl << "There's no such Lecture" << endl;

	cout << endl << "Press Enter...";
	cin.get();
}

void purchase_manipulation(string key) {
	bool exit_flag = false;
	bool action_flag = false;
	vector<int> idx;
	int n;
	while (!exit_flag) {
		string buf;
		int selection;
		clear_console();
		cout << "**************************************" << endl;
		cout << "* 1. Show My Purchases               *" << endl;
		cout << "* 2. Insert Purchase                 *" << endl;
		cout << "* 3. Modify My Purchase              *" << endl;
		cout << "* 4. Delete My Purchase              *" << endl;
		cout << "* 5. Exit                            *" << endl;
		cout << "**************************************" << endl;
		cout << endl << "Input : ";
		cin >> buf;
		cin.get();
		if (buf.size() == 1) selection = buf.at(0) - '0';
		else selection = -1;
		switch (selection) {
		case 1:
			action_flag = true; 	
			idx = search_purchase(key);
			if (idx.size() != 0) {
				cout << endl;
				for (auto it = idx.begin(); it != idx.end(); it++) {
					cout << Purchases.at(*it);
				}
			}
			else
				cout << endl << "There's no Purchase" << endl;
			break;
		case 2:
			action_flag = true;
			cout << endl << "Input Purchase ID : ";
			cin >> buf;
			cin.get();
			if (search_PID(buf)) {
				cout << "Duplicate Purchase ID" << endl;
			}
			else {
				Purchase P;
				char buffer[STDMAXBUF];
				char *ptr, *context = NULL;
				cout << endl << "Please Input Information follow format" << endl;
				cout << endl << "LectureID|MemberID|Mileage" << endl;
				cout << endl << "EX) 123456789012|MID_1000|0000009000" << endl << endl;

				cin >> buffer;
				cin.get();
				P.update_purchaseID(key);

				ptr = strtok_s(buffer, "|", &context);
				if (!ptr || !search_lecture(string(ptr))) { cout << "There's no such Lecture ID!" << endl; break; }
				P.update_lectureID(string(ptr));
				ptr = strtok_s(NULL, "|", &context);
				if (!ptr || !search_member(string(ptr))) { cout << "There's no such Lecture ID!" << endl; break; }
				if (key.compare(string(ptr))) { cout << "You Can only insert your infromation" << endl; break; }
				P.update_memberID(string(ptr));
				ptr = strtok_s(NULL, "|", &context);
				if (!ptr) { cout << "Mileage Error!" << endl; break; }
				P.update_Mileage(string(ptr));
				purchase_mod = true;
				Purchases.push_back(P);
			}
			break;
		case 3:
			action_flag = true;
			cout << endl << "Input Purchase ID : ";
			cin >> buf;
			cin.get(); 
			if (!search_PID(buf)) {
				cout << "There's no such Purchase ID" << endl;
			}
			else {
				idx = search_purchase(buf);
				string MID = Purchases.at(idx.at(0)).get_MID();
				if (key.compare(MID)) {
					cout << "It's not your purchase" << endl;
					break;
				}
				char buffer[STDMAXBUF];
				char *ptr, *context = NULL;
				cout << endl << "Please Input Information follow format" << endl;
				cout << endl << "LectureID|Mileage" << endl;
				cout << endl << "EX) 123456789012|0000009000" << endl << endl;
				Purchase P;
				P.update_memberID(MID);
				P.update_purchaseID(string(buf));
				cin >> buffer;
				cin.get();
				ptr = strtok_s(buffer, "|", &context);
				if (!ptr || !search_lecture(string(ptr))) { cout << "There's no such Lecture ID!" << endl; break; }
				P.update_lectureID(string(ptr));
				ptr = strtok_s(NULL, "|", &context);
				if (!ptr) { cout << "Mileage Error!" << endl; break; }
				P.update_Mileage(string(ptr));
				for (auto it = Purchases.begin(); it != Purchases.end(); it++) {
					if (!buf.compare(it->get_PID())) {
						*it = P;
						cout << "Update Complete" << endl;
						break;
					}
				}
			}
			break;
		case 4:
			action_flag = true;
			cout << endl << "Input Purchase ID : ";
			cin >> buf;
			cin.get();
			if (!search_PID(buf)) {
				cout << "There's no such Purchase ID" << endl;
			}
			else {
				idx = search_purchase(buf);
				string MID = Purchases.at(idx.at(0)).get_MID();
				if (key.compare(MID)) {
					cout << "It's not your purchase" << endl;
					break;
				}
				delete_purchase(buf);
			}
			break;
		case 5:return;
		default:break;
		}
		cout << endl << "If you want to CONTINUE, press Enter..." << endl;
		cin.get();
		clear_console();
	}
}

void general_menu(Member & m) {
	bool exit_flag = false;
	bool action_flag = false;
	while (!exit_flag) {
		string buf;
		int selection;
		clear_console();
		general_system();
		cout << endl << "Input : ";
		cin >> buf;
		cin.get();
		if (buf.size() == 1) selection = buf.at(0) - '0';
		else selection = -1;
		// 1 : Member Information, 2 : Search Lecture, 3 : Purchase Manipulation
		switch (selection) {
		case 1:
			action_flag = true; if(!member_information(m)) exit_flag = true; break;
		case 2:
			action_flag = true; search_lecture(); break;
		case 3:
			action_flag = true; purchase_manipulation(m.get_ID()); break;
		case 4:
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
	delete_memories();
}

bool login_window() {
	string ID, PW;
	cout << "******************************" << endl;
	cout << "* Login Window               *" << endl;
	cout << "******************************" << endl;
	cout << "ID : ";
	cin >> ID;
	cin.get();
	cout << "PW : ";
	cin >> PW;
	cin.get();
	// if admin

	make_memories();
	if (!ID.compare("admin") && !PW.compare("adminpass")) {
		delete_memories();
		admin_menu();
		return false;
	}
	else if (search_member(ID)) {
		Member &m = *(find_if(Members.begin(), Members.end(), [ID](Member &m) {
			return !ID.compare(m.Key()); 
		}));
		if (!PW.compare(m.get_PW())) {
			general_menu(m);
		}
		else {
			cout << "Login Failed" << endl;
			cout << endl << "If you want to CONTINUE, press Enter..." << endl;
			cin.get();
			delete_memories();
			clear_console();
			return false;
		}
	}
	else {
		cout << "Login Failed" << endl;
		cout << endl << "If you want to CONTINUE, press Enter..." << endl;
		cin.get();
		delete_memories();
		clear_console();
		return false;
	}
	return false;
}

void make_memories() {
	char f1[] = "fileOfMember.dat";
	char f2[] = "fileOfLecture.dat";
	char f3[] = "fileOfPurchase.dat";
	DelimFieldBuffer buf1('|'), buf2('|');
	RecordFile<Member> MemberFile(buf1);
	RecordFile<Lecture> LectureFile(buf2);

	openIndex();
	Members = Member_Index.Records;
	Lectures = Lecture_Index.Records;
	// BTree에는 데이터 파일이 없다. Purchases가 파일 대용이다.(이제까지 그랬던 것 처럼)

	Purchase P;
	PurchaseFile.Open(f3, ios::in);
	while (PurchaseFile.Read(P) != -1) {
		Purchases.push_back(P);
	}
	PurchaseFile.Close();
	PurchaseFile.Open(f3, ios::in|ios::out);
#ifdef DEBUG
	int i = 0;
	while (1) {
		Purchase P;
		int recaddr = PurchaseFile.Read(P);
		if (recaddr == -1) break;
		int res = Purchase_Index.Search(P.Key()[0]);
		char temp = Purchases.at(i).Key()[0];
	}
	PurchaseFile.Rewind();
#endif
}

void delete_memories() {
	if (member_mod) {
		Member_Index.Rewrite(const_cast<char*>("fileOfMember.dat"));
	}
	if (lecture_mod) {
		Lecture_Index.Rewrite(const_cast<char*>("fileOfLecture.dat"));
	}
	// TODO
	if (purchase_mod) {
		DelimFieldBuffer buffer('|');
		RecordFile<Purchase> PurchaseFile(buffer);
		char filename[] = "fileOfPurchase.dat";
		PurchaseFile.Create(filename, ios::out | ios::trunc);
		for (auto it = Purchases.begin(); it != Purchases.end(); it++) {
			if (PurchaseFile.Write(*it) == -1)
				cout << "Write error!" << endl;
		}
		PurchaseFile.Close();
	}
	Member_Index.Close();
	Lecture_Index.Close();
	Purchase_Index.Close();
	PurchaseFile.Close();
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
	cout << "**************************************" << endl;
	cout << "* PurchaseLectureSystem - Admin      *" << endl;
	cout << "**************************************" << endl;
	cout << "* 1. Search                          *" << endl;
	cout << "* 2. Insert                          *" << endl;
	cout << "* 3. Delete                          *" << endl;
	cout << "* 4. Update                          *" << endl;
	cout << "* 5. Exit                            *" << endl;
	cout << "**************************************" << endl;
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
				auto it = find_if(Members.begin(), Members.end(), [search_key](Member &m) {return !search_key.compare(m.Key()); });
				cout << endl << *it;
			}
			else
				cout << endl << "There's no such Member" << endl;
			break;
		case 2:
			cout << endl << " Input Lecture ID : ";
			cin >> search_key;
			cin.get();
			if (search_lecture(search_key)) {
				auto it = find_if(Lectures.begin(), Lectures.end(), [search_key](Lecture &m) {return !search_key.compare(m.Key()); });
				cout << endl << *it;
			}
			else
				cout << endl << "There's no such Lecture" << endl;
			break;
		case 3:
			cout << endl << " Input Key to search Purchase : ";
			cin >> search_key;
			cin.get();
			idx = search_purchase(search_key);
			if (search_key.size() == 1 && idx.size() != 0) {					// search by PID
				int recaddr = idx.at(0);
				Purchase P;
				PurchaseFile.Read(P, recaddr);
				cout << P;
			}
			else if (idx.size() != 0) {
				cout << endl;
				for (auto it = idx.begin(); it != idx.end(); it++) {
					cout << Purchases.at(*it);
				}
			}
			else
				cout << endl << "There's no Such Purchase" << endl;
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

void record_insert() {
	bool exit_flag = false;
	bool type_flag = false;
	purchase_mod = member_mod = lecture_mod = false;
	vector<int> idx;
	make_memories();
	while (!exit_flag) {
		string search_key;
		int type;
		clear_console();
		type = record_type("Insert");
		switch (type) {
		case 1:
			cout << endl << " Input Member ID : ";
			cin >> search_key;
			cin.get();
			if (search_member(search_key)) {
				cout << endl << "There's already duplicate Member" << endl;
			}
			else {
				if (insert_member(search_key)) {
					member_mod = true;
					cout << endl << "Insert Member Record Success" << endl;
				}
			}
			break;
		case 2:
			cout << endl << " Input Lecture ID : ";
			cin >> search_key;
			cin.get();
			if (search_lecture(search_key)) {
				cout << endl << "There's already duplicate Lecture" << endl;
			}
			else {
				if (insert_lecture(search_key)) {
					lecture_mod = true;
					cout << endl << "Insert Lecture Record Success" << endl;
				}
			}
			break;
		case 3:
			cout << endl << " Input Purchase ID : ";
			cin >> search_key;
			cin.get();
			if (search_PID(search_key)) {
				cout << endl << "There's already duplicate Purchase" << endl;
			}
			else {
				if (update_purchase(search_key)) {
					// Insertion Done(Project 3)
					Purchase P = Purchases.at(Purchases.size()-1);		// last purchase record
					int recaddr = PurchaseFile.Append(P);
					Purchase_Index.Insert(P.Key()[0], recaddr);
					cout << endl << "Insert Purchase Record Success" << endl;
				}
			}
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

void delete_member(string search_key) {
	vector<int> idx;
	int n;
	if (search_member(search_key)) {
		idx = search_purchase(search_key);
		n = 0;
		for (auto it = idx.begin(); it != idx.end(); it++) {
			//purchase_mod = true;
			Purchases.erase(Purchases.begin() + *it - n);
			n++;
		}
		RewritePurchase();
		Member_Index.Delete(const_cast<char*>(search_key.c_str()));
		member_mod = true;
		cout << endl << "Delete Member Record and " << n << " Purchase Record(s) Successfully" << endl;
	}
	else
		cout << endl << "There's no such Member" << endl;
}

void delete_lecture(string search_key) {
	vector<int> idx;
	int n;
	if (search_lecture(search_key)) {
		idx = search_purchase(search_key);
		n = 0;
		for (auto it = idx.begin(); it != idx.end(); it++) {
			//purchase_mod = true;
			Purchases.erase(Purchases.begin() + *it - n);
			n++;
		}
		RewritePurchase();
		Lecture_Index.Delete(const_cast<char*>(search_key.c_str()));
		lecture_mod = true;
		cout << endl << "Delete Lecture Record and " << n << " Purchase Record(s) Successfully" << endl;
	}
	else
		cout << endl << "There's no such Lecture" << endl;
}

void delete_purchase(string search_key) {
	vector<int> idx;
	int n;
	idx = search_purchase(search_key);
	if (search_PID(search_key)) {
		for (auto it = Purchases.begin(); it != Purchases.end(); it++) {
			if (!search_key.compare(it->get_PID())) {
				Purchases.erase(it); break;
			}
		}
		//purchase_mod = true;
		RewritePurchase();
		cout << endl << "Delete 1 Purchase Record Successfully" << endl;
	}
	else
		cout << endl << "There's no Such Purchase" << endl;
}

void RewritePurchase() {
	// Rewrite Data and Index File according to Purchases vector
	char datname[] = "fileOfPurchase.dat";
	char indname[] = "fileOfPurchase.ind";
	PurchaseFile.Close();
	Purchase_Index.Close();
	PurchaseFile.Create(datname, ios::out | ios::trunc);
	Purchase_Index.Create(indname, ios::in | ios::out | ios::trunc);
	for (auto it = Purchases.begin(); it != Purchases.end(); it++) {
		int recaddr;
		char key;
		recaddr = PurchaseFile.Write(*it);
		key = (*it).get_PID()[0];
		Purchase_Index.Insert(key, recaddr);
	}
	Purchase_Index.Close();
	Purchase_Index.Open(indname, ios::in|ios::out);
	PurchaseFile.Rewind();
}

void record_delete() {
	bool exit_flag = false;
	bool type_flag = false;
	int n;
	vector<int> idx;
	make_memories();
	while (!exit_flag) {
		string search_key;
		int type;
		clear_console();
		type = record_type("Delete");
		switch (type) {
		case 1:
			cout << endl << " Input Member ID : ";
			cin >> search_key;
			cin.get();
			delete_member(search_key);
			break;
		case 2:
			cout << endl << " Input Lecture ID : ";
			cin >> search_key;
			cin.get();
			delete_lecture(search_key);
			break;
		case 3:
			cout << endl << " Input Purchase ID : ";
			cin >> search_key;
			cin.get();
			delete_purchase(search_key);
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

void record_update() {
	bool exit_flag = false;
	bool type_flag = false;
	purchase_mod = member_mod = lecture_mod = false;
	vector<int> idx;
	make_memories();
	while (!exit_flag) {
		string search_key;
		int type;
		clear_console();
		type = record_type("Insert");
		switch (type) {
		case 1:
			cout << endl << " Input Member ID : ";
			cin >> search_key;
			cin.get();
			if (!search_member(search_key)) {
				cout << endl << "There's no Such Member" << endl;
			}
			else {
				if (update_member(search_key)) {
					member_mod = true;
					cout << endl << "Update Member Record Success" << endl;
				}
			}
			break;
		case 2:
			cout << endl << " Input Lecture ID : ";
			cin >> search_key;
			cin.get();
			if (!search_lecture(search_key)) {
				cout << endl << "There's no Such Lecture" << endl;
			}
			else {
				if (update_lecture(search_key)) {
					lecture_mod = true;
					cout << endl << "Update Lecture Record Success" << endl;
				}
			}
			break;
		case 3:
			cout << endl << " Input Purchase ID : ";
			cin >> search_key;
			cin.get();
			if (!search_PID(search_key)) {
				cout << endl << "There's no Such Purchase" << endl;
			}
			else {
				if (update_purchase(search_key)) {
					//purchase_mod = true;
					RewritePurchase();
					cout << endl << "Update Purchase Record Success" << endl;
				}
			}
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
	auto it = find_if(Members.begin(), Members.end(), [key](Member &m) {return !key.compare(m.Key()); });
	if (it != Members.end())
		return true;
	else
		return false;
}

bool search_lecture(string key) {
	auto it = find_if(Lectures.begin(), Lectures.end(), [key](Lecture &m) {return !key.compare(m.Key()); });
	if (it != Lectures.end())
		return true;
	else
		return false;
}

vector<int> search_purchase(string key) {
	vector<int> ret;
	int i = 0;
	if (key.length() == 1) {						// search by PID(key is one character in Project 3)
		int recaddr = Purchase_Index.Search(key[0]);
		if (recaddr < 0) {							// does not exist in Index
			return ret;
		}
		else {
			ret.push_back(recaddr);
			return ret;
		}
	}
	for (auto it = Purchases.begin(); it != Purchases.end(); it++, i++) {
		if (!key.compare(it->get_MID()))
			ret.push_back(i);
		else if (!key.compare(it->get_LID()))
			ret.push_back(i);
	}
	return ret;
}

bool search_PID(string key) {
	/*for (auto it = Purchases.begin(); it != Purchases.end(); it++)
		if (!key.compare(it->get_PID()))
			return true;
	return false;
	*/
	int recaddr = Purchase_Index.Search(key[0]);
	if (recaddr < 0) return false;		// does not exist
	else
		return true;
}

bool insert_lecture(string key) {
	Lecture L;
	char buffer[STDMAXBUF];
	char *ptr, *context = NULL;
	cout << endl << "Please Input Information follow format" << endl;
	cout << endl << "Subject|Level|Price|Extension|DueDate|NameOfTeacher|Textbook" << endl;
	cout << endl << "EX) FileProcessing|A|300,000|B|90|ParkSuk|FileStructure" << endl << endl;

	cin >> buffer;
	cin.get();
	L.update_ID(key);

	ptr = strtok_s(buffer, "|", &context);
	if (!ptr) { cout << "Subject Error!" << endl; return false; }
	L.update_Subject(string(ptr));
	ptr = strtok_s(NULL, "|", &context);
	if (!ptr) { cout << "Level Error!" << endl; return false; }
	L.update_level(ptr[0]);
	ptr = strtok_s(NULL, "|", &context);
	if (!ptr) { cout << "Price Error!" << endl; return false; }
	L.update_Price(commaDecimalToInt(string(ptr)));
	ptr = strtok_s(NULL, "|", &context);
	if (!ptr) { cout << "Extension Error!" << endl; return false; }
	L.update_Extension(ptr[0]);
	ptr = strtok_s(NULL, "|", &context);
	if (!ptr) { cout << "DueDate Error!" << endl; return false; }
	L.update_dueDate(commaDecimalToInt(string(ptr)));
	ptr = strtok_s(NULL, "|", &context);
	if (!ptr) { cout << "NameOfTecher Error!" << endl; return false; }
	L.update_nameOfTeacher(string(ptr));
	ptr = strtok_s(NULL, "|", &context);
	if (!ptr) { cout << "TextBook Error!" << endl; return false; }
	L.update_Textbook(string(ptr));

	Lecture_Index.Append(L);
	lecture_mod = true;
	return true;
}

bool insert_member(string key) {
	Member M;
	char buffer[STDMAXBUF];
	char *ptr, *context = NULL;
	cout << endl << "Please Input Information follow format" << endl;
	cout << endl << "Password|Name|PheonNumber|Address|Mileage" << endl;
	cout << endl << "EX) 1234|Gildong|010-1234-5678|Seoul|0000043000" << endl << endl;

	cin >> buffer;
	cin.get();
	M.update_ID(key);

	ptr = strtok_s(buffer, "|", &context);
	if (!ptr) { cout << "Password Error!" << endl; return false; }
	M.update_Password(string(ptr));
	ptr = strtok_s(NULL, "|", &context);
	if (!ptr) { cout << "Name Error!" << endl; return false; }
	M.update_Name(string(ptr));
	ptr = strtok_s(NULL, "|", &context);
	if (!ptr) { cout << "PhoneNumber Error!" << endl; return false; }
	M.update_PhoneNumber(string(ptr));
	ptr = strtok_s(NULL, "|", &context);
	if (!ptr) { cout << "Address Error!" << endl; return false; }
	M.update_Address(string(ptr));
	ptr = strtok_s(NULL, "|", &context);
	if (!ptr) { cout << "Mileage Error!" << endl; return false; }
	M.update_Mileage(string(ptr));

	Member_Index.Append(M);
	member_mod = true;
	return true;
}

bool update_member(string key) {
	Member M;
	char buffer[STDMAXBUF];
	char *ptr, *context = NULL;
	cout << endl << "Please Input Information follow format" << endl;
	cout << endl << "Password|Name|PheonNumber|Address|Mileage" << endl;
	cout << endl << "EX) 1234|Gildong|010-1234-5678|Seoul|0000043000" << endl << endl;
	
	cin >> buffer;
	cin.get();
	M.update_ID(key);
	
	ptr = strtok_s(buffer, "|", &context);
	if (!ptr) { cout << "Password Error!" << endl; return false; }
	M.update_Password(string(ptr));
	ptr = strtok_s(NULL, "|", &context);
	if (!ptr) { cout << "Name Error!" << endl; return false; }
	M.update_Name(string(ptr));
	ptr = strtok_s(NULL, "|", &context);
	if (!ptr) { cout << "PhoneNumber Error!" << endl; return false; }
	M.update_PhoneNumber(string(ptr));
	ptr = strtok_s(NULL, "|", &context);
	if (!ptr) { cout << "Address Error!" << endl; return false; }
	M.update_Address(string(ptr));
	ptr = strtok_s(NULL, "|", &context);
	if (!ptr) { cout << "Mileage Error!" << endl; return false; }
	M.update_Mileage(string(ptr));
	
	Member_Index.Update(const_cast<char*>(key.c_str()), M);
	member_mod = true;
	return true;
}

bool update_lecture(string key) {
	Lecture L;
	char buffer[STDMAXBUF];
	char *ptr, *context = NULL;
	cout << endl << "Please Input Information follow format" << endl;
	cout << endl << "Subject|Level|Price|Extension|DueDate|NameOfTeacher|Textbook" << endl;
	cout << endl << "EX) FileProcessing|A|300,000|B|90|ParkSuk|FileStructure" << endl << endl;

	cin >> buffer;
	cin.get();
	L.update_ID(key);

	ptr = strtok_s(buffer, "|", &context);
	if (!ptr) { cout << "Subject Error!" << endl; return false; }
	L.update_Subject(string(ptr));
	ptr = strtok_s(NULL, "|", &context);
	if (!ptr) { cout << "Level Error!" << endl; return false; }
	L.update_level(ptr[0]);
	ptr = strtok_s(NULL, "|", &context);
	if (!ptr) { cout << "Price Error!" << endl; return false; }
	L.update_Price(commaDecimalToInt(string(ptr)));
	ptr = strtok_s(NULL, "|", &context);
	if (!ptr) { cout << "Extension Error!" << endl; return false; }
	L.update_Extension(ptr[0]);
	ptr = strtok_s(NULL, "|", &context);
	if (!ptr) { cout << "DueDate Error!" << endl; return false; }
	L.update_dueDate(commaDecimalToInt(string(ptr)));
	ptr = strtok_s(NULL, "|", &context);
	if (!ptr) { cout << "NameOfTecher Error!" << endl; return false; }
	L.update_nameOfTeacher(string(ptr));
	ptr = strtok_s(NULL, "|", &context);
	if (!ptr) { cout << "TextBook Error!" << endl; return false; }
	L.update_Textbook(string(ptr));

	Lecture_Index.Update(const_cast<char*>(key.c_str()), L);
	lecture_mod = true;

	return true;
}

bool update_purchase(string key) {
	Purchase P;
	char buffer[STDMAXBUF];
	char *ptr, *context = NULL;
	cout << endl << "Please Input Information follow format" << endl;
	cout << endl << "LecutreID|MemberID|Mileage" << endl;
	cout << endl << "EX) 123456789012|MID_1000|0000009000" << endl << endl;

	cin >> buffer;
	cin.get();
	P.update_purchaseID(key);

	ptr = strtok_s(buffer, "|", &context);
	if (!ptr && search_lecture(string(ptr))) { cout << "LectureID Error!" << endl; return false; }
	P.update_lectureID(string(ptr));
	ptr = strtok_s(NULL, "|", &context);
	if (!ptr && search_member(string(ptr))) { cout << "MemberID Error!" << endl; return false; }
	P.update_memberID(string(ptr));
	ptr = strtok_s(NULL, "|", &context);
	if (!ptr) { cout << "Mileage Error!" << endl; return false; }
	P.update_Mileage(string(ptr));
	for (auto it = Purchases.begin(); it != Purchases.end(); it++) {
		if (!key.compare(it->get_PID())) {
			*it = P;
			return true;
		}
	}
	Purchases.push_back(P);

	return true;
}