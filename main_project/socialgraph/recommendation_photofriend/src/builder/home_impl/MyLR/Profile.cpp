/*
 * Profile.cpp
 *
 *  Created on: 2012-9-24
 *      Author: yxy
 */
#include "Profile.h"

Profile::Profile(const string& id) {

	_id = id;
	_univ = 1000; //default
	_gender = "男生";
	_birthYear = 1980;
	_city = "1111";
}

Profile::~Profile() {
}

string Profile::getId() {
	return _id;
}

int Profile::getUniv() {
	return _univ;
}

string Profile::getGender() {
	return _gender;
}

int Profile::getBirthYear() {
	return _birthYear;
}

string Profile::getCity() {
	return _city;
}

void Profile::setUniv(int univ) {
	_univ = univ;
}

void Profile::setGender(const string& gender) {
	_gender = gender;
}

void Profile::setBirthYear(int birthYear) {
	_birthYear = birthYear;
}

void Profile::setCity(const string& city) {
	_city = city;
}

