{
  #包含公共设置
  'includes':[
    'user_common.gyp',
  ],
  'variables':{
    'service_name'            : 'UserDesc',
    'service_src_path'        : '<(main_project_path)/user/<(service_name)',
  },

 'target_defaults' : {
    'include_dirs' : [
      '<(main_project_path)/third-party/redis-c-driver',
#      '/usr/local/distcache-dev/include',
#      '<(main_project_path)/tripod-new/src/cpp/include',
#      '<(main_project_path)/TalkUtil/src',
#      '<(main_project_path)/third-party/include/',
    ],
  },

  'targets' : [



  ######################################################
  {
    'target_name' : '<(service_name)',
    #'type' : 'executable',
    'type' : '<(target_mode)',
    'sources' : [
      '<!@(find <(service_src_path)/hotback/oldwrcacheagent -name "*.cpp")',
    ],
    'dependencies' : [
      './user_slice_and_adapter.gyp:*',
    ],
  }, 

  ######################################################
  {
    'target_name' : 'UserDescReader',
    #'type' : 'executable',
    'type' : '<(target_mode)',
    'sources' : [
      '<!@(find <(service_src_path)/hotback/reader -name "*.cpp")',
    ],
    'dependencies' : [
      './user_slice_and_adapter.gyp:*',
    ],
  }, 

  ######################################################
  {
    'target_name' : 'UserDescWriter',
    #'type' : 'executable',
    'type' : '<(target_mode)',
    'sources' : [
      '<!@(find <(service_src_path)/hotback/writeragent -name "*.cpp")',
    ],
    'dependencies' : [
      './user_slice_and_adapter.gyp:*',
    ],
  }, 

  ######################################################
  {
    'target_name' : 'UserContactReaderPreloader',
    #'type' : 'executable',
    'type' : '<(target_mode)',
    'sources' : [
      '<!@(find <(service_src_path)/hotback/preloader/UserContactReaderPreloader -name "*.cpp")',
    ],
    'dependencies' : [
      './user_slice_and_adapter.gyp:*',
    ],
  }, 

  ######################################################
  {
    'target_name' : 'UserTimeReaderPreloader',
    #'type' : 'executable',
    'type' : '<(target_mode)',
    'sources' : [
      '<!@(find <(service_src_path)/hotback/preloader/UserTimeReaderPreloader -name "*.cpp")',
    ],
    'dependencies' : [
      './user_slice_and_adapter.gyp:*',
    ],
  }, 



  ] #end targets
}

