{
  #包含公共设置
  'includes':[
    'buddy_common.gyp',
  ],
  'variables':{
    'service_name' : 'follow_resource_cache',
    'cache_src_path' : '<(main_project_path)/buddy/<(service_name)',
  },

  'targets' : [

  ######################################################
  {
    'target_name' : '<(service_name)',
    'type' : '<(target_mode)',
    'sources' : [
      '<(cache_src_path)/follow_resource_cache_impl.cc',
    ],
    'dependencies' : [
      './buddy_slice_and_adapter.gyp:*',
    ],
  }, #end of '<(service_name)_cache'


  ######################################################
  {
    'target_name' : '<(service_name)_test',
    'type' : 'executable',
    'sources' : [
      '<(adapter_path)/test/follow_resource_cache_test.cc',
    ],
    'dependencies' : [
      '../gtest.gyp:gtest',
      '../gtest.gyp:gtest_main',
      './buddy_slice_and_adapter.gyp:*',
    ],
  }, 

#  ######################################################
#  {
#    'target_name' : '<(service_name)_pressure_test',
#    'type' : 'executable',
#    'sources' : [
#      '<(adapter_path)/test/follow_resource_cache_pressure_test.cc',
#    ],
#    'dependencies' : [
#      '../gtest.gyp:gtest',
#      '../gtest.gyp:gtest_main',
#      './buddy_slice_and_adapter.gyp:*',
#    ],
#  }, 
#
  ######################################################
  {
    'target_name' : '<(service_name)_all',
    'type' : 'none',
    'dependencies' : [
      './buddy_slice_and_adapter.gyp:*',
      '<(service_name)',
    ],
  },


  ] #end targets
}

