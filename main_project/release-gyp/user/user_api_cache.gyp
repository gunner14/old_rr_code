{
  #包含公共设置
  'includes':[
    'user_common.gyp',
#    '../dist_cache_slice.gyp',
#    '../dist_cache_proto.gyp',
  ],
  'variables':{
    'service_name'            : 'UserApiCache',
    'service_src_path'        : '<(main_project_path)/user/<(service_name)/',
  },

  'target_defaults' : {
    'include_dirs' : [
      '/usr/local/distcache-dev/include',
      '/usr/local/distcache-util/include/',
      '/usr/local/distcache/include/',
      '<(main_project_path)/tripod-new/src/cpp/include',
      '<(main_project_path)/TalkUtil/src',
      '<(main_project_path)/third-party/include/',
      '<(main_project_path)/third-party/apr/include/apr-1',
      '<(main_project_path)/third-party/libactivemq/include/activemq-cpp-3.4.1',
      '<(main_project_path)/third-party/redis-c-driver/',
      '<(main_project_path)/message_pipe/src/cpp/',
#     '<(main_project_path)/DistCache/distributedcache/slice',
#     '<(main_project_path)/DistCache/distributedcache/protobuf',
#     '<(oce_slice_path)',
#     '<(oce_proto_path)',
    ],
    'link_settings' : {
      'libraries' : [
        '-L../third-party/libactivemq/lib',
        '-lactivemq-cpp',
        #只用xce-dev或third-party的任一个都有问题,
        '-L../third-party/apr/lib',
        '-L/usr/local/xce-dev/lib64',
        '-lapr-1', '-laprutil-1',
        '-L/usr/local/distcache-util/lib',
        '-lrdc-client',
      ],
      'ldflags': [
        '-Wl,-rpath /usr/local/xce-dev/lib64',
        '-Wl,-rpath /usr/lib64',
      ],
    },
  },




  'targets' : [


  ######################################################
  {
    'target_name' : 'DistUserApiCacheReloader',
    #'type' : 'executable',
    'type' : '<(target_mode)',
    'sources' : [
      '<!@(find <(service_src_path)/dist/reloader -name "*.cpp")',
#     '<!@(find <(main_project_path)/DistCache/distributedcache/src/main/cpp/ -name "*.cpp")',
    ],
    'dependencies' : [
      './user_slice_and_adapter.gyp:*',
    ],
  }, 

  ######################################################
  {
    'target_name' : '<(service_name)',
    #'type' : 'executable',
    'type' : '<(target_mode)',
    'sources' : [
      '<!@(find <(service_src_path)/src -name "*.cpp")',
    ],
    'dependencies' : [
      './user_slice_and_adapter.gyp:*',
    ],
  }, 

  ######################################################
  {
    'target_name' : 'UserApiCacheAgent',
    #'type' : 'executable',
    'type' : '<(target_mode)',
    'sources' : [
      '<!@(find <(service_src_path)/agent -name "*.cpp")',
    ],
    'dependencies' : [
      './user_slice_and_adapter.gyp:*',
    ],
  }, 

  ######################################################
  {
    'target_name' : 'UserApiCacheReloader',
    #'type' : 'executable',
    'type' : '<(target_mode)',
    'sources' : [
      '<!@(find <(service_src_path)/reloader -name "*.cpp")',
    ],
    'dependencies' : [
      './user_slice_and_adapter.gyp:*',
    ],
  }, 


  ######################################################
  {
    'target_name' : '<(service_name)Test',
    'type' : 'executable',
    'dependencies' : [
      '../gtest.gyp:gtest',
      '../gtest.gyp:gtest_main',
      './user_slice_and_adapter.gyp:*'
#      '../3rdparty.gyp:hiredis',
#      '../base.gyp:base',
#      '../xce_base.gyp:xce_base',
#'../tripod2.gyp:tripod_core',
    ],
    'sources' : [
    ],
  }, 

  ] #end targets
}

