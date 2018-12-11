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
	cout << endl << "Notebook loading.." << endl;
	init(Notebook);

	string phone1("89778899019");
	string phone2("89778899028");
	Contact person1("Krusenshtern", "Ivan", "Fedorovich", phone1, "seaways@mail.post", "Admiral");
	Contact person2("Chekhov", "Anton", "Pavlovich", phone2, "berrygarden@mail.post", "Writer");
	Notebook.insert(phone1, person1);
	Notebook.insert(phone2, person2);
	cout << endl << "Print table :" << endl;
	Notebook.print();
	cout << endl << "=============";
	string name("Chek");
	Notebook.erase(phone1);
	cout << endl << "Erased at hash(" << phone1 << ')' << endl;
	cout << endl << "Trying to find at hash(" << phone1 << ')' << endl;
	Notebook.find(phone1).print();
	int i = 1;
	cout << endl << "Undo : " << i << endl;
	Notebook.undo(1);
	cout << endl << "Print table :" << endl;
  Notebook.print();

	conclude(Notebook);
	return 0;
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
	ofstream fout("workbook.in");
	fout << book;
	fout.close();
}
