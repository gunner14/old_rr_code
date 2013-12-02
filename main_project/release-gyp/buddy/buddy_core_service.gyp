{
  #包含公共设置
  'includes':[
    'buddy_common.gyp',
  ],
  'variables':{
    'service_name'            : 'BuddyCore',
    'service_src_path'        : '<(main_project_path)/<(service_name)/src',
  },

  'targets' : [


  ######################################################
  {
    'target_name' : '<(service_name)',
    'type' : '<(target_mode)',
    'sources' : [
      '<!@(find <(service_src_path) -name "*.cpp")',
      '<(main_project_path)/BuddyEntry/src/BuddyEntry.cpp',
    ],
    'dependencies' : [
      './buddy_slice_and_adapter.gyp:*',
    ],
  }, 

  ] #end targets
}

