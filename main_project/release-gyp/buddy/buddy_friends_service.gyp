{
  #包含公共设置
  'includes':[
    'buddy_common.gyp',
  ],
  'variables':{
    'service_name'            : 'BuddyFriends',

    'cache_name'              : '<(service_name)Cache',
    'loader_name'             : '<(service_name)Loader',
    'preloader_name'          : '<(service_name)Preloader',

    'cache_src_path'          : '<(main_project_path)/buddy/<(cache_name)/cache',
    'loader_src_path'         : '<(main_project_path)/buddy/<(cache_name)/reloader',
    'preloader_src_path'      : '<(main_project_path)/buddy/<(cache_name)/preloader',
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
    'target_name' : '<(loader_name)',
    'type' : '<(target_mode)',
    'sources' : [
      '<!@(find <(loader_src_path) -name "*.cpp")',
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

  ] #end targets
}

