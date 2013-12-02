#include <signal.h>
#include <unistd.h>
#include "FriendRankLoaderNewAdapter.h"

void SigRoutine(int dunno);
void Initial();
void Cleanup();

int main(int argc, char* argv[])
{
	if (argc!=2)
	{
		fprintf(stderr, "To run, like this: %s <userid>\n", argv[0]);
		return -1;
	}
	time_t start = time(NULL);
	Initial();

	int uid = atoi(argv[1]);
//	printf(">>>>>>>>>>>>>> %d <<<<<<<<<<<<<<<\n", uid);
	int minid=232491085;
	int maxid=232491090;
	for(int i=232491085;i<232491090;++i)
	{
		xce::socialgraph::FriendRankLoaderAdapter::instance().reload(i);
		fprintf(stderr, "\nok. using PrintFriendRank to check whether sucessful.\n");
	}
	xce::socialgraph::FriendRankLoaderAdapter::instance().reload(1038);
	Cleanup();
	time_t end = time(NULL);
	printf("time used %ld second\n", end-start);
	return 0;
}

// SigRoutine {{{
void SigRoutine(int sig)
{
	switch (sig)
	{
		case 1:
			printf("Get a signal -- SIGHUP \n");
			break;
		case 2:
			printf("Get a signal -- SIGINT \n");
			break;
		case 3:
			printf("Get a signal -- SIGQUIT \n");
			break;
	}

	Cleanup();
	exit(0);
}// }}}
// Initial & Cleanup {{{
void Initial()
{
	signal(SIGHUP, SigRoutine); //* 下面设置三个信号的处理方法
	signal(SIGQUIT, SigRoutine);
	signal(SIGINT, SigRoutine);
}
void Cleanup()
{
	printf("\nCleaning jobs:\n");
}// }}}

