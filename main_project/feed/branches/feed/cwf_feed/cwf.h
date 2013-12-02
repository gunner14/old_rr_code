#include <boost/function.hpp>



struct Request {
  
};

namespace ctemplate {
  class TemplateDictionary {};
}

// ---------------------- some.so ---------------------
User* authorize() {}

ErrorCode g1(Request*, dict, Context* ctx) {
  // 1
  dict->Add(key, value);
  ctx->Set(0, void*);
}



struct Action {
  TemplateDictionary* Process(Request* ) {
    Task* t0 = new IceTask(&Action::TaskDone, this);
    Task* t1 = new SQLTask(&Action::TaskDone, this);
    t0->Start();

    asio::deadline_timer timer;
    timer.wait(&Timeout);

    TemplateDictionary* dict = 0;
    if (is_crucial_done()) {
      // fill dictionary
      dict = new TemplateDictionary("some name");
      dict->SetIntValue("VALUE", 42);
    }
    return dict;
  }

  void TaskDone(Task* ) {
  }

  void Timeout() {}

private:
  bool is_crucial_done() const { return false; }
  size_t count_done() const { return 3; }  
  size_t count_task() const;
};

// ---------------------- host ---------------------

// use echo-x.c
struct Response {};

struct DynamicLibrary {
  std::string filename_;
  void* handle_;
};


struct Template {};
struct Match {};

struct HostAction {
  Action* action_;
  Match* match_;
  Template* root_;
  DynamicLibrary dl_;
};

/************************************************************************

Action page_one;
page_one.Add(new IceTask(true));
page_one.Add(new IceTask(false));

FrameWork fw;
fw.Add(Pattern, DynamicLibrary("page_one.so"));

************************************************************************/

struct FrameWork {
  error_code Process(Request* request, Response* response) {
    HostAction* a = Find(Request->url());
    if (a) {
      // 1 need authorize?
      // default authorize
      //  authorize_token = get_cookie("auth")
      //  uid = service::passport::verify(authorize_token)
      //  request->set_user(new User(uid));
      TemplateDictionary* d= a->Process(request);

      if (d) {
        // a->template()->Expand(&response, d);
      }
    } else {
      // generate 404 page
    }
  }

  std::list<HostAction*> host_action_;
};

int fastcgi_main() {
  FrameWork fw;
/*
  // 1 load config file 
  //   maybe init global dict

  // 3 init HostAction(s)
  for each(regex, so, tpl in config-file)
    HostAction* ha = new // via so::action
    push_back(ha);

  // 4 Init thread pool

  // 5 Accept now

  fw.LoadConfig(string& filename)
  InitHost(bool load_action)
*/

  FCGX_Stream *in, *out, *err;
  FCGX_ParamArray envp;
  int count = 0;

  while (FCGX_Accept(&in, &out, &err, &envp) >= 0) {
    Request q;
    if (!q.Init(in, envp)) {
      // process error
    }

    Response p(out, err);
    fw.Process(&q, &p);

    FCGX_PutStr(p.str());
  }
}

/*

framework {
  global {
    KEY VALUE;
    KEY "VALUE";
  };
  authorize {
    so xxx.so;
    entry yyy;
  }
  parallel 10;

  root /path/to/template_home;
  log /path/to/file;

  404 page.html;
  403 page.html;
  302 redirect http://host/path;
};

location match-type  { ## string, search, regexp::match
  template home.tpl;
  main xxx {
    so xxx.so;
    entry xxx; // so entry
  }
  subsection xxx {
    so xxx.so;
    entry xxx; // so entry
    tolerance;
  }
  subinclude xxx {
    so xxx.so;
    entry xxx;
    tolerance;
  }

  root /path/to/template_home;
  
  timeout 100ms;
  expire max; ## 30d
  authorize {
    so xxx.so;
    entry xxx;
  };
}

//////////////////////////////////////////////////////////////////////////
location /search {     # ƥ�� url ��ʽΪ /search*
  template home.tpl;   # ��ģ���ļ�
  root /path/to/template_home; # ģ���ļ�����Ŀ¼
  timeout 100ms;
  expire max;

  header {             # ��ѡ�����û��"Ӧ��"������� Content-Length
    so search.so;
    entry Header;
  }
 
  main search {        # ������һ��Ϊ��̬���ӿ�ĺ���
   so search.so;
   entry SearchTask;
  }
  subsection menu {    # ҳ������������
    so common.so;
    entry MenuTask;
  }
  subsection footer {
    so common.so;
    entry FooterTask;
    tolerance;         # ִ�б� task ʱ����ʧ�ܣ��������������ʧ��
  }
}

action name
 ...

main {}     // request, [out] dict
header {}   // request, [out] header

section "" {} // request, section_name, parent_dict
include {}    // request, [out] dict

*/
