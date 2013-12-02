{
  #包含公共设置
  'includes':[
    'user_common.gyp',
  ],
  'variables':{
    'service_name'            : 'UserLogic',

    'service_src_path'        : '<(main_project_path)/user/<(service_name)/src',
  },

  'target_defaults' : {
    'include_dirs' : [
      '/usr/local/distcache-dev/include',
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
    'target_name' : '<(service_name)_oce_slice_and_adapter',
    'type' : 'static_library',
    'dependencies' : [
      './user_slice_and_adapter.gyp:*',
    ],
    'sources' : [
      #### MessagePipe ###################################### 
      '<!@(find <(main_project_path)/message_pipe/src/cpp -name "*.cpp")',
    ],
  }, 

  ######################################################
  {
    'target_name' : '<(service_name)',
    #'type' : 'shared_library',
    'type' : '<(target_mode)',
    'sources' : [
      '<!@(find <(service_src_path) -name "*.cpp")',
      #'<(main_project_path)/tripod-new/src/cpp/client/TripodClient.cpp',
    ],
    'dependencies' : [
      '<(service_name)_oce_slice_and_adapter',
      #'MessagePipe',
    ],
  }, 

  ######################################################
  {
    'target_name' : '<(service_name)Test',
    'type' : 'executable',
    'sources' : [
      '<(adapter_path)/test/user_logic_test.cc',
      '<(adapter_path)/UserLogicAdapter.cpp',
    ],
    'dependencies' : [
      '<(service_name)_oce_slice_and_adapter',
      '../gtest.gyp:gtest',
      '../gtest.gyp:gtest_main',
    ],
  }, 
  ] #end targets
}

