{
  #包含公共设置
  'includes':[
    'user_common.gyp',
  ],
  'variables':{
    'service_name'            : 'PersonalInfo',
    'service_src_path'        : '<(main_project_path)/user/<(service_name)/',
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
    'target_name' : 'LinkViewReader',
    #'type' : 'executable',
    'type' : '<(target_mode)',
    'sources' : [
      '<!@(find <(service_src_path)/LinkViewReader -name "*.cpp")',
    ],
    'dependencies' : [
      './user_slice_and_adapter.gyp:*',
    ],
  }, 

  ######################################################
  {
    'target_name' : 'Link2UserIdReader',
    #'type' : 'executable',
    'type' : '<(target_mode)',
    'sources' : [
      '<!@(find <(service_src_path)/Link2UserIdReader -name "*.cpp")',
    ],
    'dependencies' : [
      './user_slice_and_adapter.gyp:*',
    ],
  }, 

  ######################################################
  {
    'target_name' : 'PersonalInfoWriter',
    #'type' : 'executable',
    'type' : '<(target_mode)',
    'sources' : [
      '<!@(find <(service_src_path)/PersonalInfoWriter -name "*.cpp")',
    ],
    'dependencies' : [
      './user_slice_and_adapter.gyp:*',
    ],
  }, 

  ######################################################
  {
    'target_name' : 'LinkViewReaderPreloader',
    #'type' : 'executable',
    'type' : '<(target_mode)',
    'sources' : [
      '<!@(find <(service_src_path)/preloader/LinkViewReaderPreloader -name "*.cpp")',
    ],
    'dependencies' : [
      './user_slice_and_adapter.gyp:*',
    ],
  }, 

  ######################################################
  {
    'target_name' : 'Link2UserIdReaderPreloader',
    #'type' : 'executable',
    'type' : '<(target_mode)',
    'sources' : [
      '<!@(find <(service_src_path)/preloader/Link2UserIdReaderPreloader -name "*.cpp")',
    ],
    'dependencies' : [
      './user_slice_and_adapter.gyp:*',
    ],
  }, 


  ######################################################
  {
    'target_name' : '<(service_name)Test',
    'type' : 'executable',
    'dependencies' : [
      '../gtest.gyp:gtest',
      '../gtest.gyp:gtest_main',
      './user_slice_and_adapter.gyp:*'
#      '../3rdparty.gyp:hiredis',
#      '../base.gyp:base',
#      '../xce_base.gyp:xce_base',
#'../tripod2.gyp:tripod_core',
    ],
    'sources' : [
    ],
  }, 

  ] #end targets
}

