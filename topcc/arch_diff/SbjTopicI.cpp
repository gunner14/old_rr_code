#include "SbjTopicI.h"

using namespace MyUtil;
using namespace std;

SubjectObserverPrx SbjTopicI::getProxy() {
	
	return _proxy;
}

void SbjTopicI::subscribe(const SbjSubscriberPrx& subscriber,
                             const Ice::Current& current)  {

	try {
		getProxy()->subscribe(_topicname, subscriber, current.ctx);
	} catch (Ice::Exception& e) {
	} catch (...) {
	}
}

void SbjTopicI::unsubscribe(const SbjSubscriberPrx& subscriber, const Ice::Current& current) {

	try {
		getProxy()->unsubscribe(_topicname, subscriber, current.ctx);
	} catch (Ice::Exception& e) {
	} catch (...) {
	}
}

void SbjTopicI::run() {
	while ( true ) {
		try {
			subscribe(_subscriber);
			} catch (Ice::Exception& e) {
		} catch (std::exception& e) {
		} catch (...) {
		}
		sleep(_interval);
	}
}
