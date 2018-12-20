#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include "HashTab.h"
#include "Contact.h"

using namespace std;
typedef HashTab<string, Contact> Cellbook;

void init(Cellbook&);
void conclude(Cellbook&);

void menu(Cellbook&);
void addContact(Cellbook&);
void deleteContact(Cellbook&);
void findByPiece(Cellbook&);
void showCellbook(Cellbook&);
void undoAction(Cellbook&);

bool convertToNum(string);

int main() {
	HashTab<string, Contact> cellbook;
	
	init(cellbook);
	
	menu(cellbook);
	
	conclude(cellbook);
	
	return 0;
}

void menu(Cellbook& cellbook) {
	char action;
	while (1) {
		system("cls");
		cout << "CellBook app" << endl << endl;
		cout << "1 - Add contact" << endl;
		cout << "2 - Delete contact" << endl;
		cout << "3 - Find by substring" << endl;
		cout << "4 - Show cellbook" << endl;
		cout << "5 - Undo" << endl;
		cout << "q - Exit" << endl << endl;
		switch (action = getch()) {
			case '1': addContact(cellbook); break;
			case '2': deleteContact(cellbook); break;
			case '3': findByPiece(cellbook); break;
			case '4': showCellbook(cellbook);  break;
			case '5': undoAction(cellbook); break;
			case 'q': return;
		}
	}
	action = ' ';
}

void addContact(Cellbook& cellbook) {
	system("cls");
	string name,
		surname,
		patronymic,
		phone,
		email,
		rank;

	cout << "Name: ";
	cin >> name;
	cout << "Surname: ";
	cin >> surname;
	cout << "Patronymic: ";
	cin >> patronymic;
	cout << "Phone: ";
	cin >> phone;

	if (!convertToNum(phone))
		return;

	cout << "Email: ";
	cin >> email;
	cout << "Rank: ";
	cin >> rank;
	cout << endl << endl;

	Contact contact(surname, name, patronymic, phone, email, rank);

	enum {INSERT, EDIT};
	switch (cellbook.insert(phone, contact)) {
		case INSERT :
			cout << "Contact inserted" << endl;
			break;
		case EDIT :
			cout << "Contact edited" << endl;
	}
	system("pause");
}

void deleteContact(Cellbook& cellbook) {
	system("cls");

	string phone;
	cout << "Write the phone of the contact" << endl << "you want to delete: ";
	cin >> phone;
	cout << endl << endl;
	
	if (!convertToNum(phone))
		return;

	enum {OK, NOT};
	switch (cellbook.erase(phone)) {
		case OK :
			cout << "Deleted successfully" << endl;
			break;
		case NOT :
			cout << "Contact not found" << endl;
			break;
	}
	system("pause");
}

void findByPiece(Cellbook& cellbook) {
	system("cls");
	
	string str;
	cout << "To find: ";
	cin >> str;
	cout << endl;
	auto contact = cellbook.find_value(str);
	if (contact == cellbook.end())
		cout << "Not found" << endl;
	else {
		cout << "Found: " << endl;
		contact.print();
		cout << endl;
	}
	system("pause");
}

void showCellbook(Cellbook& cellbook) {
	system("cls");
	cellbook.print();
	system("pause");
}

void undoAction(Cellbook& cellbook) {
	system("cls");
	
	string str;

	cout << "Quantity of undo-actions: ";
	cin >> str;
	cout << endl << endl;

	if (!convertToNum(str))
		return;

	int num = stoi(str);
	if (num < 1) {
		cout << "Incorrect" << endl;
		system("pause");
		return;
	}

	cellbook.undo(num);
	cout << "Undid as much actions as it was possible" << endl;
	system("pause");
}

bool convertToNum(string str) {
	try {
		stoll(str);
	}
	catch (exception& e) {
		cout << "Incorrect" << endl;
		system("pause");
		return false;
	}
	return true;
}


void init(HashTab<string, Contact>& book) {
	ifstream fin("workbook.in");
	std::vector<std::string> sList;
	std::string s;
	while (std::getline(fin, s) && !s.empty()) {
		sList.push_back(s);
	}
	for (int i = 0; i < sList.size(); i += 6) {
		Contact person(
			sList[i],
			sList[i + 1],
			sList[i + 2],
			sList[i + 3],
			sList[i + 4],
			sList[i + 5]
		);
		book.insert(sList[i + 3], person);
	}
	fin.close();
}

void conclude(HashTab<string, Contact>& book) {
	ofstream fout("workbook.txt");
	fout << book;
	fout.close();
}
