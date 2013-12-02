#ifndef _H_IMTASKMANAGER_LEVEL_H_
#define _H_IMTASKMANAGER_LEVEL_H_

namespace com {
namespace xiaonei {
namespace talk {
namespace util {
class TaskLevel {
  TaskLevel();
 public:

  // Task level 定义，对每种业务类型定义自己的task，并在添加任务队列前
  // 首先判断队列长度，超长则不再添加新的任务
  enum {
    // 遇见业务
    TASK_MEET = 1000,

  };

};
}
;
}
;
}
;
}
;

#endif
