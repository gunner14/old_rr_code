{
  #包含公共设置
  'includes':[
    'buddy_common.gyp',
  ],
  'variables':{
    'service_name'            : 'BuddyApply',

    'cache_name'              : '<(service_name)CacheN',
    'preloader_name'          : '<(cache_name)Preloader',

    'cache_src_path'          : '<(main_project_path)/<(cache_name)/src',
    'preloader_src_path'      : '<(main_project_path)/<(cache_name)/preloader',
  },

  'targets' : [
  ######################################################
  {
    'target_name' : '<(cache_name)',
    'type' : '<(target_mode)',
    'sources' : [
      '<!@(find <(cache_src_path) -name "*.cpp")',
    ],
    'dependencies' : [
      './buddy_slice_and_adapter.gyp:*',
    ],
  }, 
  ######################################################
  {
    'target_name' : '<(preloader_name)',
    'type' : '<(target_mode)',
    'sources' : [
      '<!@(find <(preloader_src_path) -name "*.cpp")',
    ],
    'dependencies' : [
      './buddy_slice_and_adapter.gyp:*',
    ],
  }, 
  ######################################################
  {
    'target_name' : '<(service_name)CacheAdapterTest',
    'type' : 'executable',
    'sources' : [
      '<(adapter_path)/test/buddy_apply_adapter_test.cc',
    ],
    'dependencies' : [
      '../gtest.gyp:gtest',
      '../gtest.gyp:gtest_main',
      './buddy_slice_and_adapter.gyp:*',
    ],
  }, 

  ] #end targets
}

