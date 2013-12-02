{
  #包含公共设置
  'includes':[
    'user_common.gyp',
  ],
  'variables':{
    'service_name'            : 'UserScore',

    'service_src_path'        : '<(main_project_path)/score/ScoreCache/cachen',
  },

# 'target_defaults' : {
#    'include_dirs' : [
#      '/usr/local/distcache-dev/include',
#      '<(main_project_path)/tripod-new/src/cpp/include',
#      '<(main_project_path)/TalkUtil/src',
#      '<(main_project_path)/third-party/include/',
#    ],
#  },

  'targets' : [



#  ######################################################
#  {
#    'target_name' : '<(service_name)',
#    'type' : 'executable',
#    #'type' : 'shared_library',
##'type' : '<(target_mode)',
#    'sources' : [
#      '<!@(find <(service_src_path) -name "*.cpp")',
#      #'<(main_project_path)/tripod-new/src/cpp/client/TripodClient.cpp',
#    ],
#    'dependencies' : [
#      './user_slice_and_adapter.gyp:*',
#    ],
#  }, 

  ######################################################
  {
    'target_name' : '<(service_name)Test',
    'type' : 'executable',
    'dependencies' : [
      '../gtest.gyp:gtest',
      '../gtest.gyp:gtest_main',
      '../3rdparty.gyp:hiredis',
      '../base.gyp:base',
      '../xce_base.gyp:xce_base',
      './user_slice_and_adapter.gyp:*',
      '../tripod2.gyp:tripod_core',
    ],
    'sources' : [
#      '<(oce_slice_path)/ScoreCache.cpp',
      '<(adapter_path)/test/user_score_tripod_adapter_test.cc',
    ],
  }, 
  ] #end targets
}

