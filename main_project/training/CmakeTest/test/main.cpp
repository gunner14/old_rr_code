#include <iostream>
#include "CmakeTestShare.h"

using namespace std;
using namespace training;

int main() {
	CmakeTestShare *a = new CmakeTestShare();
	a->set(100);
	std::cout << "a=" << a->get() << endl;
	std::cout << "b=" << StaticShare::getB() << endl;
	return 0;
}
