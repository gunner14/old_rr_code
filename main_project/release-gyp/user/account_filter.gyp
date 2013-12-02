{
  #包含公共设置
  'includes':[
    'user_common.gyp',
  ],
  'variables':{
    'service_name'            : 'AccountFilter',

    'service_src_path'        : '<(main_project_path)/user/<(service_name)',
  },

  'targets' : [

  ######################################################
  {
    'target_name' : '<(service_name)',
    'type' : 'shared_library',
    'sources' : [
      '<!@(find <(service_src_path)/service -name "*.cpp")',
    ],
    'dependencies' : [
      './user_slice_and_adapter.gyp:*',
      '../base.gyp:base',
      '../xce_base.gyp:xce_base',
    ],
  }, 

  ######################################################
  {
    'target_name' : '<(service_name)Preloader',
    'type' : 'shared_library',
    'sources' : [
      '<!@(find <(service_src_path)/preloader -name "*.cpp")',
    ],
    'dependencies' : [
      './user_slice_and_adapter.gyp:*',
      '../base.gyp:base',
      '../xce_base.gyp:xce_base',
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
  ] #end targets
}

