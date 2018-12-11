#include "Contact.h"

Contact::Contact(const string& newSurname, const string& newName,
	const string& newFatherName, const string& newNumber,
	const string& newMail, const string& newRank) {
	this->surname = newSurname;
  this->name = newName;
	this->patronymic = newFatherName;
	this->number = newNumber;
	this->mail = newMail;
	this->rank = newRank;
}

ostream& operator<< (ostream& out, const Contact& right) {
	out << right.surname << endl;
	out << right.name << endl;
	out << right.patronymic << endl;
	out << right.number << endl;
	out << right.mail << endl;
	out << right.rank << endl;
	return out;
}


bool Contact::find_by_param(const string& subStr) {
	auto pos = this->surname.find(subStr);
	if (pos != string::npos) return true;
  pos = this->name.find(subStr);
	if (pos != string::npos) return true;
	pos = this->patronymic.find(subStr);
	if (pos != string::npos) return true;
	pos = this->number.find(subStr);
	if (pos != string::npos) return true;
	pos = this->mail.find(subStr);
	if (pos != string::npos) return true;
	pos = this->number.find(subStr);
	return pos != string::npos;
}
