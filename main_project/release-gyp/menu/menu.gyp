{
  #包含公共设置
  'includes':[
    'menu_common.gyp',
  ],
  'variables':{
    'service_name'            : 'MenuCache',

    'service_src_path'        : '<(main_project_path)/app/MenuCache',
  },

 'target_defaults' : {
    'include_dirs' : [
      '/usr/local/distcache-dev/include',
      '/usr/local/distcache/include', #ugly!
      '/usr/local/distcache-util/include',
      '<(main_project_path)/tripod-new/src/cpp/include',
      '<(main_project_path)/TalkUtil/src',
      '<(main_project_path)/third-party/include/',
      '<(main_project_path)/third-party/apr/include/apr-1',
      '<(main_project_path)/third-party/libactivemq/include/activemq-cpp-3.4.1',
      '<(main_project_path)/third-party/redis-c-driver/',
      '<(main_project_path)/message_pipe/src/cpp/',
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
    'target_name' : 'MenuCache',
    'type' : 'shared_library',
    'dependencies' : [
      '../xce_base.gyp:xce_base',
    ],

    'sources' : [
      '<(service_src_path)/replica/MenuCacheManagerI.cpp',
      '<(adapter_path)/MenuCacheReplicaAdapter.cpp',
      '<(adapter_path)/ControllerAdapter.cpp',
      '<(oce_slice_path)/DataSync.cpp',
      '<(oce_slice_path)/MenuCache.cpp',
      '<(oce_proto_path)/DataSync.pb.cc',
    ],
  }, 
  
  ######################################################
  {
    'target_name' : 'MenuCacheTripod',
    'type' : 'shared_library',
    'dependencies' : [
      '../xce_base.gyp:xce_base',
      '../tripod.gyp:tripod_base',
    ],

    'sources' : [
      '<(service_src_path)/MenuTripod/MenuCacheManagerI.cpp',
      '<(service_src_path)/MenuTripod/MenuTripodShare.cpp',
      '<(adapter_path)/ControllerAdapter.cpp',
    ],
  },
  
  ######################################################
  {
    'target_name' : 'MenuDBOperator',
    'type' : 'executable',
    'dependencies' : [
      '../xce_base.gyp:xce_base',
      '../tripod.gyp:tripod_base',
    ],

    'sources' : [
      '<(service_src_path)/test/main.cpp',
      '<(service_src_path)/MenuTripod/MenuTripodShare.cpp',
      '<(adapter_path)/ControllerAdapter.cpp',
    ],
  },
  
#  ######################################################
#  {
#    'target_name' : '<(service_name)_oce_slice_and_adapter',
#    'type' : 'static_library',
#    'dependencies' : [
#     '../tripod2.gyp:tripod_core',
#    ],
#    'sources' : [
#      
#      #### Cpp Files Generated from ICE ##################### 
#      '<(oce_slice_path)/FootprintUnite.cpp',
#
#      #### Adapter Cpp Files ################################ 
#      '<(adapter_path)/i_like_it_cache_tripod_adapter.cc'
#    ],
#  }, 



#  ######################################################
#  {
#    'target_name' : '<(service_name)',
#    'type' : 'executable',
#    #'type' : 'shared_library',
##'type' : '<(target_mode)',
#    'sources' : [
#      '<!@(find <(service_src_path) -name "*.cpp")',
#      #'<(main_project_path)/tripod-new/src/cpp/client/TripodClient.cpp',
#    ],
#    'dependencies' : [
#      '<(service_name)_oce_slice_and_adapter',
#    ],
#  }, 

  ] #end targets
}

