#pragma once

#include <string>

using namespace std;

class Contact {
public:
	Contact() = default;
	Contact(const string& surname, const string& name, const string& patronymic,
		const string& number, const string& mail, const string& rank);
		bool find_by_param(const string&);
	friend ostream& operator<< (ostream& out, const Contact& right);
private:
	string surname,
				 name,
				 patronymic,
				 number,
				 mail,
				 company,
				 rank;
};
