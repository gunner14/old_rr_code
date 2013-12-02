{
  #包含公共设置
  'includes':[
    'buddy_common.gyp',
  ],
  'variables':{
    'service_name'            : 'BuddyByAddTime',

    'cache_name'              : '<(service_name)Cache',
    'loader_name'             : '<(service_name)Loader',
    'preloader_name'          : '<(service_name)Preloader',

    'cache_src_path'          : '<(main_project_path)/buddy/<(cache_name)/cache',
    'loader_src_path'         : '<(main_project_path)/buddy/<(cache_name)/loader',
    'preloader_src_path'      : '<(main_project_path)/buddy/<(cache_name)/preloader',
  },

  'targets' : [


  ######################################################
  {
    'target_name' : '<(cache_name)',
    'type' : '<(target_mode)',
    'sources' : [
      '<(cache_src_path)/<(cache_name)I.cpp',
    ],
    'dependencies' : [
      './buddy_slice_and_adapter.gyp:*',
    ],
  }, 

  ######################################################
  {
    'target_name' : '<(loader_name)',
    'type' : '<(target_mode)',
    'sources' : [
      '<(loader_src_path)/<(loader_name)I.cpp',
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
      '<(preloader_src_path)/<(preloader_name)I.cpp',
    ],
    'dependencies' : [
      './buddy_slice_and_adapter.gyp:*',
    ],
  }, 
  ######################################################
  {
    'target_name' : '<(service_name)Test',
    'type' : 'executable',
    'sources' : [
      '<(adapter_path)/test/buddy_by_addtime_adapter_test.cc',
    ],
    'dependencies' : [
      '../gtest.gyp:gtest',
      '../gtest.gyp:gtest_main',
      './buddy_slice_and_adapter.gyp:*',
    ],
  }, 

  ] #end targets
}

