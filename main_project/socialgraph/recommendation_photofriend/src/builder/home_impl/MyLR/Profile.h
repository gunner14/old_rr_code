/*
 * Profile.h
 *
 *  Created on: 2012-9-24
 *      Author: yxy
 */

#ifndef PROFILE_H_
#define PROFILE_H_
#include <string>
using namespace std;

class Profile {
public:
	Profile(const string& id);
	~Profile();
	string getId();
	int getUniv();
	string getGender();
	int getBirthYear();
	string getCity();

	void setUniv(int univ);
	void setGender(const string& gender);
	void setBirthYear(int birthYear);
	void setCity(const string& city);


private:
	string _id;
	int _univ;
	string _gender;
	int _birthYear;
	string _city;

};

#endif /* PROFILE_H_ */
