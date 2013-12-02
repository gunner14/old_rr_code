#ifndef EXCEPTION_MANAGER_H_
#define EXCEPTION_MANAGER_H_

#include <string>

#include "client/linux/handler/exception_handler.h"

#include "LogWrapper.h"

//----------------------------------------------------------

static bool dumpCallback(const char* dump_path,
												 const char* minidump_id,
												 void* context,
												 bool succeeded) 
{
	MCE_INFO("Dump paht: " << dump_path << "/" << minidump_id);
	return succeeded;
}

//----------------------------------------------------------

/*@brief create minidump file when process crash*/
class ExceptionManager {
 public:
	ExceptionManager(const std::string& path = "/data/xce/XSocialGraph/dump") :
		eh_(path, NULL, dumpCallback, NULL, true) {
	}

	virtual ~ExceptionManager() {}

 private:
	google_breakpad::ExceptionHandler eh_;
};

#endif
