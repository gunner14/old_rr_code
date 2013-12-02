{
  #包含公共设置
  'includes':[
    'user_common.gyp',
  ],
  'variables':{
    'service_name'            : 'IdCache',
    'reader_service_name'     : '<(service_name)Reader',
    'writer_service_name'     : '<(service_name)Writer',
    'service_src_path'        : '<(main_project_path)/user/<(service_name)/',
    'reader_src_path'         : '<(main_project_path)/user/<(service_name)/reader',
    'writer_src_path'         : '<(main_project_path)/user/<(service_name)/writer',
    'share_src_path'          : '<(main_project_path)/user/<(service_name)/share',
    'preloader_src_path'      : '<(main_project_path)/user/<(service_name)/preloader',
  },

 'target_defaults' : {
    'include_dirs' : [
      '<(main_project_path)/third-party/redis-c-driver',
#      '/usr/local/distcache-dev/include',
#      '<(main_project_path)/tripod-new/src/cpp/include',
#      '<(main_project_path)/TalkUtil/src',
#      '<(main_project_path)/third-party/include/',
    ],
  },

  'targets' : [


  ######################################################
  {
    'target_name' : '<(reader_service_name)',
    #'type' : 'executable',
    'type' : '<(target_mode)',
    'sources' : [
      '<!@(find <(reader_src_path) -name "*.cpp")',
    #  '<!@(find <(share_src_path) -name "*.cpp")',
    ],
    'dependencies' : [
      './user_slice_and_adapter.gyp:*',
    ],
  }, 

  ######################################################
  {
    'target_name' : '<(writer_service_name)',
    #'type' : 'executable',
    'type' : '<(target_mode)',
    'sources' : [
      '<!@(find <(writer_src_path) -name "*.cpp")',
     # '<!@(find <(share_src_path) -name "*.cpp")',
    ],
    'dependencies' : [
      './user_slice_and_adapter.gyp:*',
    ],
  }, 

  ######################################################
  {
    'target_name' : 'DoubleAccountId2RelatedIdPreloader',
    #'type' : 'executable',
    'type' : '<(target_mode)',
    'sources' : [
      '<!@(find <(preloader_src_path)/DoubleAccountId2RelatedIdPreloader -name "*.cpp")',
      #'<!@(find <(share_src_path) -name "*.cpp")',
    ],
    'dependencies' : [
      './user_slice_and_adapter.gyp:*',
    ],
  }, 

  ######################################################
  {
    'target_name' : 'VirtualId2RelatedIdPreloader',
    #'type' : 'executable',
    'type' : '<(target_mode)',
    'sources' : [
      '<!@(find <(preloader_src_path)/VirtualId2RelatedIdPreloader -name "*.cpp")',
      #'<!@(find <(share_src_path) -name "*.cpp")',
    ],
    'dependencies' : [
      './user_slice_and_adapter.gyp:*',
    ],
  }, 

  ######################################################
  {
    'target_name' : 'id_cache_reader_adapter_test',
    'type' : 'executable',
    'dependencies' : [
      '../gtest.gyp:gtest',
      '../gtest.gyp:gtest_main',
      '../base.gyp:base',
      '../xce_base.gyp:xce_base',
    ],
    'sources' : [
      '<(adapter_path)/IdCacheReaderAdapter.cpp',
      '<(oce_slice_path)/IdCache.cpp',
      '<(adapter_path)/test/id_cache_reader_adapter_test.cc',
    ],
  }, 

  ] #end targets
}

