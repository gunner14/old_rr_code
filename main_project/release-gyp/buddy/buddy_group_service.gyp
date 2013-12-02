{
  #包含公共设置
  'includes':[
    'buddy_common.gyp',
  ],
  'variables':{
    'service_name'            : 'BuddyGroup',
    'service_src_path'        : '<(main_project_path)/<(service_name)/src',
  },

  'targets' : [


  ######################################################
  {
    'target_name' : '<(service_name)',
    'type' : '<(target_mode)',
    'sources' : [
      '<!@(find <(service_src_path) -name "*.cpp")',
    ],
    'dependencies' : [
      './buddy_slice_and_adapter.gyp:*',
      '../3rdparty.gyp:breakpad',
    ],
    'include_dirs':[
      '../../third-party/google-breakpad/src/',
    ],
    'link_settings' : {
       'libraries' : [ 
       ],
    },
  }, 

  ######################################################
  {
    'target_name' : '<(service_name)Test',
    'type' : 'executable',
    'dependencies' : [
      '../gtest.gyp:gtest',
      '../gtest.gyp:gtest_main',
      './buddy_slice_and_adapter.gyp:*',
    ],
    'sources' : [
 #     '<(adapter_path)/test/buddy_group_adapter_test.cc',
    ],
  }, 

  ] #end targets
}

