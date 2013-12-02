{
  #包含公共设置
  'includes':[
    'user_common.gyp',
  ],
  'variables':{
    'service_name'            : 'VipInfo',
    'service_src_path'        : '<(main_project_path)/user/<(service_name)/',
  },

 'target_defaults' : {
     'dependencies' : [
      '../base.gyp:base',
      '../xce_base.gyp:xce_base',
      '../tripod2.gyp:tripod_core',
    ],
    'include_dirs' : [
      '/usr/local/distcache-dev/include',
      '<(main_project_path)/tripod-new/src/cpp/include',
      '<(main_project_path)/third-party/include/',
      '<(main_project_path)/third-party/apr/include/apr-1',
      '<(main_project_path)/third-party/libactivemq/include/activemq-cpp-3.4.1',
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
    'target_name' : 'VipInfoWriter',
    #'type' : 'executable',
    'type' : '<(target_mode)',
    'sources' : [
      '<!@(find <(service_src_path)/writer -name "*.cpp")',
      '<!@(find <(main_project_path)/message_pipe/src/cpp -name "*.cpp")',
    ],
    'dependencies' : [
      './user_slice_and_adapter.gyp:*',
    ],
  },

  ######################################################
  {
    'target_name' : 'VipMemberDataReaderPreloader',
    #'type' : 'executable',
    'type' : '<(target_mode)',
    'sources' : [
      '<!@(find <(service_src_path)/preloader/VipMemberDataReaderPreloader -name "*.cpp")',
    ],
    'dependencies' : [
      './user_slice_and_adapter.gyp:*',
    ],
  },

  ######################################################
  {
    'target_name' : 'Icon2UrlDataReaderPreloader',
    #'type' : 'executable',
    'type' : '<(target_mode)',
    'sources' : [
      '<!@(find <(service_src_path)/preloader/Icon2UrlDataReaderPreloader -name "*.cpp")',
    ],
    'dependencies' : [
      './user_slice_and_adapter.gyp:*',
    ],
  },


  ######################################################
  {
    'target_name' : 'HatDataReaderPreloader',
    #'type' : 'executable',
    'type' : '<(target_mode)',
    'sources' : [
      '<!@(find <(service_src_path)/preloader/HatDataReaderPreloader -name "*.cpp")',
    ],
    'dependencies' : [
      './user_slice_and_adapter.gyp:*',
    ],
  },


  ######################################################
  {
    'target_name' : 'VipInfoReader',
    #'type' : 'executable',
    'type' : '<(target_mode)',
    'sources' : [
      '<!@(find <(service_src_path)/reader -name "*.cpp")',
    ],
    'dependencies' : [
      './user_slice_and_adapter.gyp:*',
    ],
  },


  ######################################################
  {
    'target_name' : 'vip_icon_data_tripod_adapter_test',
    'type' : 'executable',
    'dependencies' : [
      '../gtest.gyp:gtest',
      '../gtest.gyp:gtest_main',
      '../3rdparty.gyp:hiredis',
      '../base.gyp:base',
      '../xce_base.gyp:xce_base',
      '../tripod2.gyp:tripod_core',
    ],
    'sources' : [
    ],
  }, 
  ] #end targets
}

