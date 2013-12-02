{
  #包含公共设置
  'includes':[
    'user_common.gyp',
  ],
  'variables':{
    'service_name'            : 'UserBase',

    'service_src_path'        : '<(main_project_path)/user/<(service_name)',
  },

  'targets' : [

  ######################################################
  {
    'target_name' : 'reload_tool',
    'type' : 'executable',
    'sources' : [
      
 
      '<(main_project_path)/tools/ReloadTool/main.cpp',
    ],
    'dependencies' : [
      './user_slice_and_adapter.gyp:*',
      '../3rdparty.gyp:hiredis',
      '../base.gyp:base',
      '../xce_base.gyp:xce_base',
      '../tripod2.gyp:tripod_core',
    ],
  }, 



  ######################################################
  {
    'target_name' : '<(service_name)',
    'type' : 'shared_library',
    'sources' : [
      '<!@(find <(service_src_path)/hotback/oldwr -name "*.cpp")',
        ],
    'dependencies' : [
      './user_slice_and_adapter.gyp:*',
      '../3rdparty.gyp:hiredis',
      '../base.gyp:base',
      '../xce_base.gyp:xce_base',
      '../tripod2.gyp:tripod_core',
    ],
  }, 

  ######################################################
  {
    'target_name' : '<(service_name)Reader',
    'type' : 'shared_library',
    'sources' : [
      '<!@(find <(service_src_path)/hotback/reader -name "*.cpp")',
    ],
    'dependencies' : [
      './user_slice_and_adapter.gyp:*',
    ],
  }, 

# ######################################################
# {
#   'target_name' : 'user_passport_tripod_adapter_test',
#   'type' : 'executable',
#   'dependencies' : [
#     '../gtest.gyp:gtest',
#     '../gtest.gyp:gtest_main',
#     '../3rdparty.gyp:hiredis',
#     '../base.gyp:base',
#     '../xce_base.gyp:xce_base',
#     '../tripod2.gyp:tripod_core',
#   ],
#   'sources' : [
#     '<(adapter_path)/UserPassportTripodAdapter.cpp',
#     '<(oce_slice_path)/UserBase.cpp',
#     '<(adapter_path)/test/user_passport_tripod_adapter_test.cc',
#   ],
# }, 
# {
#   'target_name' : 'user_name_tripod_adapter_test',
#   'type' : 'executable',
#   'dependencies' : [
#     '../gtest.gyp:gtest',
#     '../gtest.gyp:gtest_main',
#     '../3rdparty.gyp:hiredis',
#     '../base.gyp:base',
#     '../xce_base.gyp:xce_base',
#     '../tripod2.gyp:tripod_core',
#   ],
#   'sources' : [
#     '<(adapter_path)/UserNameTripodAdapter.cpp',
#     '<(oce_slice_path)/UserBase.cpp',
#     '<(adapter_path)/test/user_name_tripod_adapter_test.cc',
#   ],
# }, 
# {
#   'target_name' : 'user_right_tripod_adapter_test',
#   'type' : 'executable',
#   'dependencies' : [
#     '../gtest.gyp:gtest',
#     '../gtest.gyp:gtest_main',
#     '../3rdparty.gyp:hiredis',
#     '../base.gyp:base',
#     '../xce_base.gyp:xce_base',
#     '../tripod2.gyp:tripod_core',
#   ],
#   'sources' : [
#     '<(adapter_path)/UserRightTripodAdapter.cpp',
#     '<(oce_slice_path)/UserBase.cpp',
#     '<(adapter_path)/test/user_right_tripod_adapter_test.cc',
#   ],
# }, 
# {
#   'target_name' : 'user_state_tripod_adapter_test',
#   'type' : 'executable',
#   'dependencies' : [
#     '../gtest.gyp:gtest',
#     '../gtest.gyp:gtest_main',
#     '../3rdparty.gyp:hiredis',
#     '../base.gyp:base',
#     '../xce_base.gyp:xce_base',
#     '../tripod2.gyp:tripod_core',
#   ],
#   'sources' : [
#     '<(adapter_path)/UserStateTripodAdapter.cpp',
#     '<(oce_slice_path)/UserBase.cpp',
#     '<(adapter_path)/test/user_state_tripod_adapter_test.cc',
#   ],
# }, 
# {
#   'target_name' : 'user_logincount_tripod_adapter_test',
#   'type' : 'executable',
#   'dependencies' : [
#     '../gtest.gyp:gtest',
#     '../gtest.gyp:gtest_main',
#     '../3rdparty.gyp:hiredis',
#     '../base.gyp:base',
#     '../xce_base.gyp:xce_base',
#     '../tripod2.gyp:tripod_core',
#   ],
#   'sources' : [
#     '<(adapter_path)/UserLoginCountTripodAdapter.cpp',
#     '<(oce_slice_path)/UserBase.cpp',
#     '<(adapter_path)/test/user_logincount_tripod_adapter_test.cc',
#   ],
# }, 
# {
#   'target_name' : 'user_born_tripod_adapter_test',
#   'type' : 'executable',
#   'dependencies' : [
#     '../gtest.gyp:gtest',
#     '../gtest.gyp:gtest_main',
#     '../3rdparty.gyp:hiredis',
#     '../base.gyp:base',
#     '../xce_base.gyp:xce_base',
#     '../tripod2.gyp:tripod_core',
#   ],
#   'sources' : [
#     '<(adapter_path)/UserBornTripodAdapter.cpp',
#     '<(oce_slice_path)/UserBase.cpp',
#     '<(adapter_path)/test/user_born_tripod_adapter_test.cc',
#   ],
# }, 
# {
#   'target_name' : 'user_stage_tripod_adapter_test',
#   'type' : 'executable',
#   'dependencies' : [
#     '../gtest.gyp:gtest',
#     '../gtest.gyp:gtest_main',
#     '../3rdparty.gyp:hiredis',
#     '../base.gyp:base',
#     '../xce_base.gyp:xce_base',
#     '../tripod2.gyp:tripod_core',
#   ],
#   'sources' : [
#     '<(adapter_path)/UserStageTripodAdapter.cpp',
#     '<(oce_slice_path)/UserBase.cpp',
#     '<(adapter_path)/test/user_stage_tripod_adapter_test.cc',
#   ],
# }, 
# {
#   'target_name' : 'user_doing_tripod_adapter_test',
#   'type' : 'executable',
#   'dependencies' : [
#     '../gtest.gyp:gtest',
#     '../gtest.gyp:gtest_main',
#     '../3rdparty.gyp:hiredis',
#     '../base.gyp:base',
#     '../xce_base.gyp:xce_base',
#     '../tripod2.gyp:tripod_core',
#   ],
#   'sources' : [
#     '<(adapter_path)/UserDoingTripodAdapter.cpp',
#     '<(oce_slice_path)/UserBase.cpp',
#     '<(adapter_path)/test/user_doing_tripod_adapter_test.cc',
#   ],
# }, 
# {
#   'target_name' : 'user_url_tripod_adapter_test',
#   'type' : 'executable',
#   'dependencies' : [
#     '../gtest.gyp:gtest',
#     '../gtest.gyp:gtest_main',
#     '../3rdparty.gyp:hiredis',
#     '../base.gyp:base',
#     '../xce_base.gyp:xce_base',
#     '../tripod2.gyp:tripod_core',
#   ],
#   'sources' : [
#     '<(adapter_path)/UserUrlTripodAdapter.cpp',
#     '<(oce_slice_path)/UserBase.cpp',
#     '<(adapter_path)/test/user_url_tripod_adapter_test.cc',
#   ],
# }, 
# {
#   'target_name' : 'user_base_tripod_adapter_test',
#   'type' : 'executable',
#   'dependencies' : [
#     '../gtest.gyp:gtest',
#     '../gtest.gyp:gtest_main',
#     '../3rdparty.gyp:hiredis',
#     '../base.gyp:base',
#     '../xce_base.gyp:xce_base',
#     '../tripod2.gyp:tripod_core',
#   ],
#   'sources' : [
#     '<(adapter_path)/UserBaseTripodAdapter.cpp',
#     '<(oce_slice_path)/UserBase.cpp',
#     '<(adapter_path)/test/user_base_tripod_adapter_test.cc',
#   ],
# }, 
# {
#   'target_name' : 'user_basic_tripod_adapter_test',
#   'type' : 'executable',
#   'dependencies' : [
#     '../gtest.gyp:gtest',
#     '../gtest.gyp:gtest_main',
#     '../3rdparty.gyp:hiredis',
#     '../base.gyp:base',
#     '../xce_base.gyp:xce_base',
#     '../tripod2.gyp:tripod_core',
#   ],
#   'sources' : [
#     '<(adapter_path)/UserBasicTripodAdapter.cpp',
#     '<(oce_slice_path)/UserBase.cpp',
#     '<(adapter_path)/test/user_basic_tripod_adapter_test.cc',
#   ],
# }, 
  ] #end targets
}

