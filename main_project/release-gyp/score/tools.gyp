{
  #包含公共设置
  'includes':[
    'score_common.gyp',
  ],
  'variables':{
    'service_name'            : 'tools',

    'service_src_path'        : '<(main_project_path)/score/ScoreCache/tools',
  },

 'target_defaults' : {
    'include_dirs' : [
      '/usr/local/distcache-dev/include',
      '/usr/local/distcache/include', #ugly!
      '/usr/local/distcache-util/include',
      '<(main_project_path)/tripod-new/src/cpp/include',
      '<(main_project_path)/TalkUtil/src',
      '<(main_project_path)/third-party/include/',
      '<(main_project_path)/third-party/apr/include/apr-1',
      '<(main_project_path)/third-party/libactivemq/include/activemq-cpp-3.4.1',
      '<(main_project_path)/third-party/redis-c-driver/',
      '<(main_project_path)/message_pipe/src/cpp/',
    ],
    'link_settings' : {
      'libraries' : [
        '-L../third-party/libactivemq/lib',
        '-lactivemq-cpp',
        #只用xce-dev或third-party的任一个都有问题,
        '-L../third-party/apr/lib',
        '-L/usr/local/xce-dev/lib64',
        '-lapr-1', '-laprutil-1',
        '-L/usr/local/distcache-util/lib',
        '-lrdc-client',
      ],
      'ldflags': [
        '-Wl,-rpath /usr/local/xce-dev/lib64',
        '-Wl,-rpath /usr/lib64',
      ],
    },
  },

  'targets' : [

  ######################################################
  {
    'target_name' : 'ValidTool',
    'type' : 'executable',
    'dependencies' : [
      '../xce_base.gyp:xce_base',
    ],

    'sources' : [
      '<(service_src_path)/ValidTool.cpp',
      '<(oce_slice_path)/ScoreCache.cpp',
    ],
  }, 

  ######################################################
  {
    'target_name' : 'LoaderTool',
    'type' : 'executable',
    'dependencies' : [
      '../xce_base.gyp:xce_base',
      '../base.gyp:base',
    ],

    'sources' : [
      '<(service_src_path)/LoaderTool.cpp',
      '<(oce_slice_path)/ScoreCache.cpp',
      '<(adapter_path)/ScoreCacheAdapter.cpp',
      '<(main_project_path)/score/ScoreCache/share/ScoreCacheDataFactory.cpp',
      '<(main_project_path)/DbCxxPool/src/ConnectionPoolManager.cpp',
    ],
  }, 


  ] #end targets
}

