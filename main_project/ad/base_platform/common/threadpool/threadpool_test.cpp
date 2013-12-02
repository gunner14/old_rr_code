#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <iostream>
#include "threadpool.h"



using namespace std;

void  TestEngineU(void*  param)
{
   cout << "TestEngineU" << endl;  
}

int main(int argc, char **argv){
    threadpool tp;
    int i = 0;
    tp = create_threadpool(20);
    for(; i < 16; i ++){
				string data;
        dispatch(tp, TestEngineU, (void *) i);
    }
    destroy_threadpool(tp);
    
    cout << "**main** done first\n";
		sleep(5);
    exit(-1);
    return 0;
}


