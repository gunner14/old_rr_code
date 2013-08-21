#include <IceUtil/IceUtil.h>

using namespace std;

void test_thread(){
}

class ProduceThread: virtual public IceUtil::Thread {
	private:
		vector<int>& _queIds;
		IceUtil::Monitor<IceUtil::Mutex>& _queMutex;
	public:
		ProduceThread( vector<int>& queIds,
				IceUtil::Monitor<IceUtil::Mutex>& queMutex ) :
			_queIds(queIds),
			_queMutex(queMutex) {
			}
		virtual void run(){
			while ( true ){
				{
					IceUtil::Monitor<IceUtil::Mutex>::Lock lock( _queMutex );
					if ( _queIds.size() == 1 ){
						_queMutex.wait();
						cout << "haha,ProduceThread liveing, size = " << _queIds.size() << endl;
						continue;
					}
					_queIds.push_back(999);
					if ( _queIds.size() == 1 ){
						_queMutex.notify();
						cout << "haha,ProduceThread notifying, size = " << _queIds.size() << endl;
					}
				}
			}
		}
};
class ConsumeThread: virtual public IceUtil::Thread {
	private:
		vector<int>& _queIds;
		IceUtil::Monitor<IceUtil::Mutex>& _queMutex;
	public:
		ConsumeThread( vector<int>& queIds,
				IceUtil::Monitor<IceUtil::Mutex>& queMutex ) :
			_queIds(queIds),
			_queMutex(queMutex) {
			}
		virtual void run(){
			while ( true ){
				{
					IceUtil::Monitor<IceUtil::Mutex>::Lock lock( _queMutex );
					if ( _queIds.size() == 0 ){
						_queMutex.wait();
						cout << "haha,ConsumeThread liveing, size = " << _queIds.size() << endl;
						continue;
					}
					_queIds.clear();
					if ( _queIds.size() == 0 ){
						_queMutex.notify();
						cout << "haha,ConsumeThread notifying, size = " << _queIds.size() << endl;
						continue;
					}
				}
			}
		}
};
class TestThread{
public:
	TestThread(){
		cout << "starting" << endl;
		ProduceThread _pt(_queIds,_queMutex);
		ConsumeThread _ct(_queIds,_queMutex);
		_pt.start(128*1024).detach();
		_ct.start(128*1024).detach();
		cout << "started" << endl;
	}
private:
	vector<int> _queIds;
	IceUtil::Monitor<IceUtil::Mutex> _queMutex;
};
