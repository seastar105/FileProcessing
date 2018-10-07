#include"test.h"

using namespace std;

void showMember() {
	Member ptr;
	ifstream ifs("listOfMember.txt");
	if (ifs.fail()) {
		cout << "File Open error" << endl;
		return;
	}
	int n;
	string buf;

	ifs >> n;
	ifs.ignore(numeric_limits<streamsize>::max(), '\n');
	cout << "(ID,PASSWORD,NAME,PHONENUBMER,ADDRESS,MILEAGE)" << endl;
	for (int i = 0; i < 10; i++) {
		ifs >> ptr;
		cout << ptr;
	}
	ifs.close();
}

void showLecture() {
	Lecture ptr;
	ifstream ifs("listOfLecture.txt");
	if (ifs.fail()) {
		cout << "File Open error" << endl;
		return;
	}
	int n;
	string buf;

	ifs >> n;
	ifs.ignore(numeric_limits<streamsize>::max(), '\n');
	cout << "(ID,SUBJECT,LEVEL,PRICE,EXTENSION,DUE DATE,TEACHER,TEXTBOOK)" << endl;
	for (int i = 0; i < 10; i++) {
		ifs >> ptr;
		cout << ptr;
	}
	ifs.close();
}

void showPurchase() {
	Purchase ptr;
	ifstream ifs("listOfPurchase.txt");
	if (ifs.fail()) {
		cout << "File Open error" << endl;
		return;
	}
	int n;
	string buf;

	ifs >> n;
	ifs.ignore(numeric_limits<streamsize>::max(), '\n');
	cout << "(PurchaseID,LecutureID,MemberID,Mileage)" << endl;
	for (int i = 0; i < 10; i++) {
		ifs >> ptr;
		cout << ptr;
	}
	ifs.close();
}

void MemberTest() {
	ifstream ifs("listOfMember.txt");
	if (ifs.fail()) {
		cout << "File Open error" << endl;
		return ;
	}
	int n;

	ifs >> n;
	ifs.ignore(numeric_limits<streamsize> ::max(), '\n');

	DelimFieldBuffer Buffer('|', STDMAXBUF);
	RecordFile <Member> MemberFile(Buffer);

	char filename[] = "fileOfMember.dat";
	MemberFile.Create(filename, ios::out | ios::trunc);
	for (int i = 0; i < n; i++) {
		Member m;
		ifs >> m;
		int recaddr;
		if ((recaddr = MemberFile.Write(m)) == -1)
			cout << "Write Error!" << endl;
		/*
		else
			cout << "Write at " << recaddr << endl;
		*/
	}
	MemberFile.Close();

	MemberFile.Open(filename, ios::in);
	cout << "(ID,PASSWORD,NAME,PHONENUBMER,ADDRESS,MILEAGE)" << endl;
	for (int i = 0; i < 10; i++) {
		Member m;
		MemberFile.Read(m);
		cout << m;
	}
	MemberFile.Close();
}

void LectureTest() {
	ifstream ifs("listOfLecture.txt");
	if (ifs.fail()) {
		cout << "File Open error" << endl;
		return;
	}
	int n;

	ifs >> n;
	ifs.ignore(numeric_limits<streamsize> ::max(), '\n');

	DelimFieldBuffer Buffer('|', STDMAXBUF);
	RecordFile <Lecture> LectureFile(Buffer);

	char filename[] = "fileOfLecture.dat";
	LectureFile.Create(filename, ios::out | ios::trunc);
	for (int i = 0; i < n; i++) {
		Lecture L;
		ifs >> L;
		int recaddr;
		if ((recaddr = LectureFile.Write(L)) == -1)
			cout << "Write Error!" << endl;
		/*
		else
			cout << "Write at " << recaddr << endl;
		*/
	}
	LectureFile.Close();

	LectureFile.Open(filename, ios::in);
	cout << "(ID,SUBJECT,LEVEL,PRICE,EXTENSION,DUE DATE,TEACHER,TEXTBOOK)" << endl;
	for (int i = 0; i < 10; i++) {
		Lecture L;
		LectureFile.Read(L);
		cout << L;
	}
	LectureFile.Close();
}

void PurchaseTest() {
	ifstream ifs("listOfPurchase.txt");
	if (ifs.fail()) {
		cout << "File Open error" << endl;
		return;
	}
	int n;

	ifs >> n;
	ifs.ignore(numeric_limits<streamsize> ::max(), '\n');

	DelimFieldBuffer Buffer('|', STDMAXBUF);
	RecordFile <Purchase> PurchaseFile(Buffer);

	char filename[] = "fileOfPurchase.dat";
	PurchaseFile.Create(filename, ios::out | ios::trunc);
	for (int i = 0; i < n; i++) {
		Purchase P;
		ifs >> P;
		int recaddr;
		if ((recaddr = PurchaseFile.Write(P)) == -1)
			cout << "Write Error!" << endl;
		/*
		else
		cout << "Write at " << recaddr << endl;
		*/
	}
	PurchaseFile.Close();

	PurchaseFile.Open(filename, ios::in);
	cout << "(PurchaseID,LecutureID,MemberID,Mileage)" << endl;
	for (int i = 0; i < 10; i++) {
		Purchase P;
		PurchaseFile.Read(P);
		cout << P;
	}
	PurchaseFile.Close();
}