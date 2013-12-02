#include "feed/feedview/modifytime.h"

#include <time.h>
#include <stdio.h>
#ifdef WIN32
#define snprintf _snprintf
#endif

#include "ctemplate/template.h"

#include "feed/base_feed/basictypes.h"
#include "feed/base_feed/logging.h"
#include "feed/base_feed/startuplist.h"
#include "feed/base_feed/globalinit.h"
#include "feed/cwf_feed/frame.h"
#include "feed/base_feed/signals.h"
#include "IceLogger.h"
#include "feed/feedview/expandfeed.h"
#include "feed/feedview/feed_hdf_parser.h"
#include "feed/feedview/ambcache.h"
#include "feed/feedview/whitelist.h"

namespace xce {

// java version:
// http://svn.d.xiaonei.com/ugc/feed/src/main/java/com/xiaonei/platform/component/xfeed/display/template/directives/LongTimeFormartDirective.java

void FriendlyTimeModifier::Modify(const char* in, size_t inlen,
    const ctemplate::PerExpandData*,
    ctemplate::ExpandEmitter* out,
    const std::string& arg) const {
#if 0
      int min = (int) ( (d.getTime() - time) / (60 * 1000));
      if(min <= 0){
        out.write("刚刚更新");
        return ;
      }else if(min < 60){
        out.write(min+"分钟前");
        return ;
      }else if(min < 1440){
        int iHour = (int) ( (d.getTime() - time) / (60 * 60 * 1000));
        out.write(iHour+"小时前");
      }else if(min >=1440 && min < 4320){
        SimpleDateFormat format = new SimpleDateFormat("HH:mm");
        int iDay = (int) ( (d.getTime() - time) / (24 * 60 * 60 * 1000));
        d.setTime(time);
        out.write(iDay+"天前  " + format.format(d));
      }else{
        d.setTime(time);
        SimpleDateFormat format = new SimpleDateFormat("MM-dd HH:mm");
        String date = format.format(d);
      }
#endif

#ifdef WIN32
  #define atoll    _atoi64
#endif
  // 不需要复制 in，已经是一个c string

  // uint64 t = atoll(in);
  long long t = atoll(in);

  t = t / 1000; // java中的time是毫秒

  if (t < 10000)
    return;

  char buf[0x100];

  time_t now = time(0);
  int diff_minutes = (now - t) / 60;
  if (diff_minutes <= 0) {
    int sec = now - t;
    int len = 0;
    if(sec <= 0) {
      len = snprintf(buf, sizeof(buf), "刚刚更新");
    }
    else {
      len = snprintf(buf, sizeof(buf), "%d秒钟前", sec);
    }
    out->Emit(buf, len);
  }
  else if (diff_minutes < 60) {
    int len = snprintf(buf, sizeof(buf), "%d分钟前", diff_minutes);
    out->Emit(buf, len);
  }
  else {
    struct tm timeinfo;
    struct tm nowtimeinfo;
#ifdef WIN32
    int err = localtime_s(&timeinfo, (const time_t *)&t);
    if (err)
      return;
    err = localtime_s(&nowtimeinfo, (const time_t *)&now);
    if (err)
      return;
#elif defined(POSIX)
    localtime_r((const time_t *)&t, &timeinfo); // TODO: 如何得知用户的时区
    localtime_r((const time_t *)&now, &nowtimeinfo);
#endif
    int len = 0;
    int year = nowtimeinfo.tm_year - timeinfo.tm_year;
    int day = nowtimeinfo.tm_yday - timeinfo.tm_yday;
    if(year >= 1) {
      len = strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", &timeinfo);
    }   
    else {
      if(day < 1) {
        len = strftime(buf, sizeof(buf), "今天 %H:%M", &timeinfo);
      }   
      else if(day == 1) {
        len = strftime(buf, sizeof(buf), "昨天 %H:%M", &timeinfo);
      }   
      else {
        len = strftime(buf, sizeof(buf), "%m-%d %H:%M", &timeinfo);
      }   
    }   
    out->Emit(buf, len);
  }
}

FriendlyTimeModifier friendly_time;

void FriendlyTimelineTimeModifier::Modify(const char* in, size_t inlen,
    const ctemplate::PerExpandData*,
    ctemplate::ExpandEmitter* out,
    const std::string& arg) const {

#ifdef WIN32
  #define atoll    _atoi64
#endif

  long long t = atoll(in);
  t = t / 1000; // java中的time是毫秒
  if (t < 10000)
    return;
  char buf[0x100];
  struct tm timeinfo;
#ifdef WIN32
    int err = localtime_s(&timeinfo, (const time_t *)&t);
    if (err)
      return;
#elif defined(POSIX)
    localtime_r((const time_t *)&t, &timeinfo);
#endif
    int len = 0;
    len = strftime(buf, sizeof(buf), "%Y-%m-%d", &timeinfo);
    out->Emit(buf, len);
}

FriendlyTimelineTimeModifier friendly_timeline_time;

void FriendlyXJSModifier::Modify(const char* in, size_t inlen,
    const ctemplate::PerExpandData*,
    ctemplate::ExpandEmitter* out,
    const std::string& arg) const {
  if(inlen > 5 * 1024) {
    return; 
  } 
  char buf[5 * 1024 + 1] = {0};
  char *ptr;
  int x = 0;
  if(in[0] == '"' && in[inlen - 1] == '"') {
    ++ x;
    -- inlen; 
    while (x < inlen) {
      ptr = strchr(in + x, '\\');
      if (ptr == NULL || (ptr-in >= inlen)) {
        strncat(buf, in + x, inlen-x);
        x = inlen;
      } else {
        strncat(buf, in + x, (ptr - in) - x);
        x = ptr - in + 1; //x指向\后的字符
        if(x < inlen) {
          switch (in[x]) {
          case '"': case '\\': case '/': strncat(buf, in + x, 1); break;
          case 'b': strncat(buf, "\b", 1); break;
          case 't': strncat(buf, "\t", 1); break;
          case 'n': strncat(buf, "\n", 1); break;
          case 'f': strncat(buf, "\f", 1); break;
          case 'r': strncat(buf, "\r", 1); break;
          default: break;
          }
        }
        ++ x;
      }
    }
  } else {
    strncpy(buf, in, inlen);
  }
  out->Emit(buf, strlen(buf));
}

FriendlyXJSModifier friendly_xjs;

static void SignalReload(int) {
  MCE_INFO("reload template");
  ctemplate::Template::ReloadAllIfChanged();

  int is_test = g_test_mode;
  MCE_INFO("reload cs template");
  xce::FeedTplCache::Instance().Reload(is_test);
  if (!xce::FeedSchemaCache::Instance().Reload(is_test)) {
    MCE_WARN("FeedHdfParser load error!");
  }
}

#define PRODUCT_SIGNAL_AMB_LOAD_UID 61
static void SignalReload2(int) {
  MyUtil::TaskManager::instance().execute(new feed::AmbInitTask());
}
#define FEED_WHITELIST_LOAD_UID 62
static void SignalReload3(int) {
  MyUtil::TaskManager::instance().execute(new feed::WhiteInitTask());
}

// TODO: ubb replace DoingUtil.getInstance().replace(s);

// 静态函数可能添加不上，需要main内部再显示添加一次
static void Init() {
  ctemplate::AddModifier("x-time", &friendly_time);
  ctemplate::AddModifier("x-js-escape", &friendly_xjs);
  ctemplate::AddModifier("x-tltime-format", &friendly_timeline_time);

  // TODO: move to perfect place
  InstallSignal(XCE_SIGNAL_CWF_LOAD_TEMPLATE, &SignalReload);
  InstallSignal(PRODUCT_SIGNAL_AMB_LOAD_UID, &SignalReload2);
  InstallSignal(FEED_WHITELIST_LOAD_UID, &SignalReload3);

  MyUtil::TaskManager::instance().execute(new feed::AmbInitTask());
  MyUtil::TaskManager::instance().scheduleRepeated(new feed::AmbLoadTask());
  MyUtil::TaskManager::instance().execute(new feed::WhiteInitTask());
}

GLOBAL_INIT(MODIFYTIME, {
  AddStartup(&Init);
});

}
