{
  #包含公共设置
  'includes':[
    'buddy_common.gyp',
  ],
  #变量定义
  'variables':{
    'service_name'            : 'BuddyCoreLogic',

    'service_src_path'        : '<(main_project_path)/<(service_name)',
  },

  #编译目标
  'targets' : [

  ######################################################
  {
    'target_name' : '<(service_name)',
    #'type' : 'shared_library',
    'type' : '<(target_mode)',
    'sources' : [
      '<!@(find <(service_src_path) -name "*.cpp")',
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
      '<(adapter_path)/test/friend_relation_notifier_test.cc',
    ],
    'dependencies' : [
      './buddy_slice_and_adapter.gyp:*',
    ],
  }, 
  ] #end targets
}

