#include <iostream>
#include <fstream>
#include <string>
#include "HashTab.h"
#include "Contact.cpp"

using namespace std;

void init(HashTab<string, Contact>&);
void conclude(HashTab<string, Contact>&);

int main() {
	HashTab<string, Contact> Notebook;
	init(Notebook);
	Contact person1("Ivan", "Krusenshtern", "Fedorovich", "89778899249", "seaways@mail.post", "Admiral");
	Contact person2("Anton", "Chekhov", "Pavlovich", "89778899247", "berrygarden@mail.post", "Writer");
	string phone1("89778899249");
	string phone2("89778899247");
	Notebook.insert(phone1, person1);
	Notebook.insert(phone2, person2);
	Notebook.print();
	string name("Chek");
	//Notebook.find_value(name).print();
	Notebook.erase(phone1);
	Notebook.find(phone1).print();
	Notebook.undo(1);
	conclude(Notebook);
	return 0;
}

void init(HashTab<string, Contact>& book) {
	ifstream fin("workbook.txt");
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

// void execute() {
// 	do {
// 		system("cls");
// 		cout << "1 - add contact" << endl;
// 		cout << "2 - delete contact" << endl;
// 		cout << "3 - change contact" << endl;
// 		cout << "4 - find contact" << endl;
// 	} while
// }

void conclude(HashTab<string, Contact>& book) {
	ofstream fout("workbook.txt");
	fout << book;
	fout.close();
}
