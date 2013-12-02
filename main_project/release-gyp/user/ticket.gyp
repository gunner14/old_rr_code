{
  #包含公共设置
  'includes':[
    'user_common.gyp',
  ],
  'variables':{
    'service_name'            : 'Ticket',
    'service_src_path'        : '<(main_project_path)/tripod_service/<(service_name)',
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
    'target_name' : 'Tripod<(service_name)Manager',
    'type' : 'shared_library',
    'sources' : [
      '<!@(find <(service_src_path)/manager -name "*.cpp")',
    ],
    'dependencies' : [
      '../3rdparty.gyp:hiredis',
      '../tripod2.gyp:tripod_core',
      '../base.gyp:base',
      '../xce_base.gyp:xce_base',
      './user_slice_and_adapter.gyp:*',
      '<(service_name)_oce_slice_and_adapter',
    ],
  }, 

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

  ] #end targets
}

