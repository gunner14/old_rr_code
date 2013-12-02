#include <iostream>

#include "FeedSortReplicaAdapter.h"

using namespace std;
using namespace xce::feed;

template <class T>
void test(T& t, const string& name) {
    int cluster = t.getCluster();
    for (int i=0; i<cluster; ++i) {
        try {
            t.Sort(i, NULL);
            cout << i << "." << name << ".Sort ok." << endl;
        } catch (Ice::Exception e) {
            cout << i << "." << name << ".Sort err: " << e << endl;
        }
    }
}
int main(int argc, char* argv[])
{
    FeedSortReplicaAdapter1& feedchannel = FeedSortReplicaAdapter1::instance();
    FeedSortReplicaAdapter2& ctrlchannel = FeedSortReplicaAdapter2::instance();
    string cmd = "";
    // while (cmd != "q") {
    while (true) {
        test(feedchannel, "feedchannel");
        test(ctrlchannel, "ctrlchannel");
        sleep(1);
        // cin >> cmd;
    }
    return 0;
}
