{
  #包含公共设置
  'includes':[
    'buddy_common.gyp',
  ],
  'variables':{
    'service_name' : 'BuddyById',
    'cache_src_path' : '<(main_project_path)/buddy/BuddyByIdCache/cache',
    'loader_src_path' : '<(main_project_path)/buddy/BuddyByIdCache/loader',
  },

  'targets' : [
  ######################################################
  {
    'target_name' : 'BuddyByIdCache',
    'type' : '<(target_mode)',
    'sources' : [
      '<(cache_src_path)/BuddyByIdCacheI.cpp',
    ],
    'dependencies' : [
      './buddy_slice_and_adapter.gyp:*',
    ],
  }, #end BuddyByIdCahce

  ######################################################
  {
    'target_name' : 'BuddyByIdLoader',
    'type' : '<(target_mode)',
    'sources' : [
      '<(loader_src_path)/BuddyByIdLoaderI.cpp',
    ],
    'dependencies' : [
      './buddy_slice_and_adapter.gyp:*',
    ],
  }, #end BuddyByIdCahce

  ] #end targets
}

