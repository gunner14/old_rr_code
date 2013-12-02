{
  #包含公共设置
  'includes':[
    'score_common.gyp',
  ],
  'variables':{
    'service_name'            : 'ScoreCache',

    'service_src_path'        : '<(main_project_path)/score/ScoreCache',
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
    'target_name' : 'score_guarder',
    'type' : 'executable',
    'dependencies' : [
      '../xce_base.gyp:xce_base',
    ],

    'sources' : [
      '<(service_src_path)/guarder/pc_impl.cc',
      '<(service_src_path)/guarder/score_guarder.cc',
      '<(adapter_path)/ScoreCacheAdapter.cpp',
      '<(oce_slice_path)/ScoreCache.cpp',
    ],
  }, 

  ######################################################
  {
    'target_name' : 'ScoreCacheN',
    'type' : 'shared_library',
    'dependencies' : [
      '../xce_base.gyp:xce_base',
    ],

    'sources' : [
      '<(service_src_path)/cachen/ScoreCacheNI.cpp',
      '<(service_src_path)/share/ScoreCacheDataFactory.cpp',
      '<(oce_slice_path)/ScoreCache.cpp',
    ],
  }, 

  ######################################################
  {
    'target_name' : 'ScoreLoaderN',
    'type' : 'shared_library',
    'dependencies' : [
      '../xce_base.gyp:xce_base',
    ],

    'sources' : [
      '<(service_src_path)/loadern/ScoreLoaderNI.cpp',
      '<(service_src_path)/loadern/ScoreChecker.cpp',
      '<(service_src_path)/share/ScoreCacheDataFactory.cpp',
      '<(adapter_path)/ScoreCacheAdapter.cpp',
      '<(adapter_path)/UserScoreEventAdapter.cpp',
      '<(adapter_path)/ControllerAdapter.cpp',
      '<(oce_slice_path)/ScoreCache.cpp',
      '<(oce_slice_path)/UserScoreEvent.cpp',
      '<(oce_slice_path)/DataSync.cpp',
      '<(oce_proto_path)/DataSync.pb.cc',
    ],
  }, 

  ######################################################
  {
    'target_name' : 'ScorePreloader',
    'type' : 'shared_library',
    'dependencies' : [
      '../xce_base.gyp:xce_base',
    ],

    'sources' : [
      '<(service_src_path)/preloader/ScorePreloaderI.cpp',
      '<(service_src_path)/share/ScoreCacheDataFactory.cpp',
      '<(oce_slice_path)/ScoreCache.cpp',
    ],
  }, 

  ######################################################
  {
    'target_name' : 'ScoreCheckWorker',
    'type' : 'executable',
    'dependencies' : [
      '../xce_base.gyp:xce_base',
    ],

    'sources' : [
      '<(service_src_path)/checker/ScoreCheckWorker.cpp',
      '<(service_src_path)/checker/Main.cpp',
      '<(service_src_path)/share/ScoreCacheDataFactory.cpp',
      '<(adapter_path)/ScoreCacheAdapter.cpp',
      '<(oce_slice_path)/ScoreCache.cpp',
    ],
  }, 

  ######################################################
  {
    'target_name' : 'LoginRecordWorker',
    'type' : 'executable',
    'dependencies' : [
      '../xce_base.gyp:xce_base',
    ],

    'sources' : [
      '<(service_src_path)/../LoginRecordWorker/src/Main.cpp',
      '<(service_src_path)/../LoginRecordWorker/src/LoginRecordWorker.cpp',
#      '<(service_src_path)/share/ScoreCacheDataFactory.cpp',
      '<(adapter_path)/ScoreCacheAdapter.cpp',
      '<(adapter_path)/UserStateAdapter.cpp',
      '<(oce_slice_path)/ScoreCache.cpp',
      '<(oce_slice_path)/UserBase.cpp',
    ],
  }, 


  ######################################################
  {
  #########score code has errors, fail to complie#######
    'target_name' : 'ScoreTest',
    'type' : 'executable',
    'dependencies' : [
      '../xce_base.gyp:xce_base',
    ],

    'sources' : [
      '<(service_src_path)/test/main.cpp',
      '<(service_src_path)/share/ScoreCacheDataFactory.cpp',
      '<(adapter_path)/ScoreCacheAdapter.cpp',
      '<(oce_slice_path)/ScoreCache.cpp',
    ],
  }, 



  ] #end targets
}

