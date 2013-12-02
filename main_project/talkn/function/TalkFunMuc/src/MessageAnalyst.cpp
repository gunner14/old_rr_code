#include "MessageAnalyst.h"
#include "TaskManager.h"
#include "TalkFunTask.h"

using namespace com::xiaonei::talk::function::muc;
using namespace MyUtil;

/*说明：根据传入参数，判断调用具体功能的task
 *参数: 传入群聊服务的参数
 */
void MessageAnalyst::handle(const Str2StrMap &paras) {
  Str2StrMap::const_iterator ns_iter;
  if ((ns_iter=paras.find("iq.query:xmlns")) != paras.end()) {
    Str2StrMap::const_iterator config_type_iter;
    string config_type;
    if ((config_type_iter=paras.find("iq:type")) != paras.end())
      config_type = config_type_iter->second;
    if (ns_iter->second == "http://conference.talk.renren.com/create") 
      TaskManager::instance().execute(new CreateGroupTask(paras));
    else if (ns_iter->second == "http://conference.talk.renren.com/search") 
      TaskManager::instance().execute(new SerachGroupTask(paras));
    else if (ns_iter->second == "http://conference.talk.renren.com/grouplist") 
      TaskManager::instance().execute(new GetGroupListTask(paras));
    else if (ns_iter->second == "http://conference.talk.renren.com/offlinemessage") 
      TaskManager::instance().execute(new OfflineMessageTask(paras));
    else if (ns_iter->second == "http://conference.talk.renren.com/items") 
      TaskManager::instance().execute(new GetMembersTask(paras));
    else if (ns_iter->second == "http://conference.talk.renren.com/config") { 
      if (config_type == "set")
        TaskManager::instance().execute(new SetGroupConfigTask(paras));
      else if (config_type == "get")
        TaskManager::instance().execute(new GetGroupConfigTask(paras));
    } else if (ns_iter->second == "http://conference.talk.renren.com/destroy") 
      TaskManager::instance().execute(new DestroyGroupTask(paras));
    else if (ns_iter->second == "http://conference.talk.renren.com/toplimit") 
      TaskManager::instance().execute(new GetGroupLimitInfoTask(paras));
    else if (ns_iter->second == "http://conference.talk.renren.com/admin") 
      TaskManager::instance().execute(new ChangePrivilegeTask(paras));
    else if (ns_iter->second == "http://conference.talk.renren.com/kick") 
      TaskManager::instance().execute(new KickUsersTask(paras));
    else if (ns_iter->second == "http://conference.talk.renren.com/quit") 
      TaskManager::instance().execute(new QuitGroupTask(paras));
  } else if ((ns_iter=paras.find("presence.priority"))!=paras.end()) { //下线 
      TaskManager::instance().execute(new ChangeStatusPresenceTask(paras));
  } else if ((ns_iter=paras.find("presence.x:xmlns")) != paras.end()) {
    if (ns_iter->second == "http://conference.talk.renren.com/offline") 
      TaskManager::instance().execute(new  ChangeStatusPresenceTask(paras));
  } else if ((ns_iter=paras.find("message.x:xmlns")) != paras.end()) {
    if (ns_iter->second == "http://conference.talk.renren.com/apply") 
      TaskManager::instance().execute(new ApplyToJoinGroupTask(paras));
    else if (ns_iter->second == "http://conference.talk.renren.com/transfer") 
      TaskManager::instance().execute(new TransferGroupTask(paras));
    else if (ns_iter->second == "http://conference.talk.renren.com/invite") 
      TaskManager::instance().execute(new InviteToJoinGroupTask(paras));
  } else if  ((ns_iter=paras.find("message:type")) != paras.end()) {
    if (ns_iter->second == "mucchat")
      TaskManager::instance().execute(new SendMessageTask(paras));
    else if (ns_iter->second == "chatext")
      TaskManager::instance().execute(new AttachmentMessageTask(paras));
  }
    else  
      TaskManager::instance().execute(new Tester1Task(paras));
}
