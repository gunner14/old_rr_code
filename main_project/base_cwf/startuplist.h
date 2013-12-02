#ifndef XCE_BASE_STARTUP_LIST_H__
#define XCE_BASE_STARTUP_LIST_H__

namespace xce {

// C++ 未对全局静态变量的调用顺序做保证
// 特别静态变量间的依赖会导致问题

typedef void (*StartupFunction)(void);

void AddStartup(StartupFunction, bool first = false);
void RunStartupList();
void DestoryStartupList();


}
#endif // XCE_BASE_STARTUP_LIST_H__
