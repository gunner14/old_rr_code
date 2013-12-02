{
  #包含公共设置
  'includes':[
    'buddy_common.gyp',
  ],
  'variables':{
    'service_name' : 'follow_relation',
    'cache_src_path' : '<(main_project_path)/buddy/<(service_name)/cache',
    'writer_src_path' : '<(main_project_path)/buddy/<(service_name)/writer',
  },

  'targets' : [

  ######################################################
  {
    'target_name' : '<(service_name)_cache',
    'type' : '<(target_mode)',
    'sources' : [
      '<(cache_src_path)/follow_relation_cache_impl.cc',
    ],
    'dependencies' : [
      './buddy_slice_and_adapter.gyp:*',
    ],
  }, #end of '<(service_name)_cache'

  ######################################################
  {
    'target_name' : '<(service_name)_writer',
    'type' : '<(target_mode)',
    'sources' : [
      '<(writer_src_path)/follow_relation_writer_impl.cc',
    ],
    'dependencies' : [
      './buddy_slice_and_adapter.gyp:*',
    ],
  }, #end of '<(service_name)_writer' 

  ######################################################
  {
    'target_name' : '<(service_name)_cache_test',
    'type' : 'executable',
    'sources' : [
      '<(adapter_path)/test/follow_relation_cache_test.cc',
    ],
    'dependencies' : [
      '../gtest.gyp:gtest',
      '../gtest.gyp:gtest_main',
      './buddy_slice_and_adapter.gyp:*',
    ],
  }, 

  ######################################################
  {
    'target_name' : '<(service_name)_cache_pressure_test',
    'type' : 'executable',
    'sources' : [
      '<(adapter_path)/test/follow_relation_cache_pressure_test.cc',
    ],
    'dependencies' : [
      '../gtest.gyp:gtest',
      '../gtest.gyp:gtest_main',
      './buddy_slice_and_adapter.gyp:*',
    ],
  }, 

  ######################################################
  {
    'target_name' : '<(service_name)_writer_test',
    'type' : 'executable',
    'sources' : [
      '<(adapter_path)/test/follow_relation_writer_test.cc',
    ],
    'dependencies' : [
      '../gtest.gyp:gtest',
      '../gtest.gyp:gtest_main',
      './buddy_slice_and_adapter.gyp:*',
    ],
  },

  ######################################################
  {
    'target_name' : '<(service_name)_writer_pressure_test',
    'type' : 'executable',
    'sources' : [
      '<(adapter_path)/test/follow_relation_writer_pressure_test.cc',
    ],
    'dependencies' : [
      '../gtest.gyp:gtest',
      '../gtest.gyp:gtest_main',
      './buddy_slice_and_adapter.gyp:*',
    ],
  },
  ######################################################

  {
    'target_name' : '<(service_name)_test',
    'type' : 'executable',
    'sources' : [
      '<(adapter_path)/test/follow_relation_test.cc',
    ],
    'dependencies' : [
      './buddy_slice_and_adapter.gyp:*',
    ],
  }, 

  ######################################################
  {
    'target_name' : '<(service_name)_all',
    'type' : 'none',
    'dependencies' : [
      './buddy_slice_and_adapter.gyp:*',
      '<(service_name)_cache',
      '<(service_name)_writer',
      '<(service_name)_cache_test',
      '<(service_name)_cache_pressure_test',
      '<(service_name)_writer_test',
      '<(service_name)_writer_pressure_test',
      '<(service_name)_test',
    ],
  },


  ] #end targets
}

