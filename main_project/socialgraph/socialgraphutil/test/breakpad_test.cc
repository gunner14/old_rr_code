#include <iostream>

#include "socialgraph/socialgraphutil/exception_manager.h"

void crash() {
	volatile int* a = (int*)(NULL);
	*a = 1;
}

int main(int argc, char* argv[]) {
	ExceptionManager e(".");
	crash();
	sleep(2);
	return 0;
}
