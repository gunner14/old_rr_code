{
  'includes':[
    'buddy_common.gyp',
  ],

  'target_defaults' : {
    'include_dirs' : [
      '/usr/local/distcache-dev/include',
      '/usr/local/distcache-util/include/',
      '/usr/local/distcache/include/',
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

  'variables':{
    'service_name' : 'JourneyLogic',
    'service_path' : '<(main_project_path)/Journey',
  },
  
  'targets' : [ 
  {
    'target_name' : 'JourneyLogic',
    'type' : 'shared_library',
    'sources' : [
      '<(service_path)/JourneyLogic.cpp',
      '<(service_path)/JourneyLogicTripodAdapter.cpp',
    ],
    'dependencies':[
      './buddy_slice_and_adapter.gyp:*',
    ],
  },

  ]#end targets
}
