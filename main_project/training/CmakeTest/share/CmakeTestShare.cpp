#include <iostream>
#include "CmakeTestShare.h"

using namespace std;
using namespace training;

int CmakeTestShare::get() {
	return _a;
}

void CmakeTestShare::set(int a) {
	_a = a;
}

bool StaticShare::init() {
	_b = 520;
	return true;
}

int StaticShare::getB() {
	return _b;
}

int StaticShare::_b;
bool StaticShare::_init = StaticShare::init();
