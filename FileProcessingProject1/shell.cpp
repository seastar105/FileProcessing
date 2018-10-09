#include"shell.h"

using namespace std;

void shell() {
	bool exit_flag = false;
	bool admin_flag = false;
	while (!exit_flag) {
		int selection;
		string buf;

		menu();

		cin >> buf;
		cin.get();
		if(buf.size() == 1) selection = buf.at(0) - '0';
		else selection = -1;
		switch (selection) {
		case 1:showMember(); break;
		case 2:showLecture(); break;
		case 3:showPurchase(); break;
		case 4:MemberTest(); break;
		case 5:LectureTest(); break;
		case 6:PurchaseTest(); break;
		case 7:admin_flag = true; LecturePurchaseSystem(); break;
		case 8:exit_flag = true; return;
		default:
			cout << endl << "Please input correctly" << endl;  break;
		}
		if (!admin_flag) {
			cout << endl;
			cout << "If you want to CONTINUE, press Enter..." << endl;
			cin.get();
		}

		clear_console();
	}
}

void menu() {
	cout << "******************************" << endl;
	cout << "*  File Processing Project 1 *" << endl;
	cout << "******************************" << endl;
	cout << "* 1. ShowMember              *" << endl;
	cout << "* 2. ShwoLecture             *" << endl;
	cout << "* 3. ShowPurchase            *" << endl;
	cout << "* 4. MemberTest              *" << endl;
	cout << "* 5. LectureTest             *" << endl;
	cout << "* 6. PurchaseTest            *" << endl;
	cout << "* 7. LecturePurchaseSystem   *" << endl;
	cout << "* 8. EXIT                    *" << endl;
	cout << "******************************" << endl;
}
