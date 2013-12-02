{
  #包含公共设置
  'includes':[
    'user_common.gyp',
  ],
  'variables':{
    'service_name'            : 'AccountConnect',

    'service_src_path'        : '<(main_project_path)/user/<(service_name)',
  },

  'targets' : [

  ######################################################
  {
    'target_name' : '<(service_name)',
    'type' : 'shared_library',
    'include_dirs' : [
      '<(main_project_path)/third-party/redis-c-driver/',
    ],
    'sources' : [
      '<!@(find <(service_src_path)/service -name "*.cpp")',
    ],
    'dependencies' : [
      '../3rdparty.gyp:hiredis',
      '../tripod2.gyp:tripod_core',
      '../base.gyp:base',
      '../xce_base.gyp:xce_base',
      './user_slice_and_adapter.gyp:*',
    ],
  }, 

  ] #end targets
}

