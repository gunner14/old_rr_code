{
  #包含公共设置
  'includes':[
    'like_common.gyp',
  ],
  'variables':{
    'service_name'            : 'ILikeIt',

    'service_src_path'        : '<(main_project_path)/app/ILikeIt',
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
    'target_name' : '<(service_name)_oce_slice_and_adapter',
    'type' : 'static_library',
    'dependencies' : [
     '../tripod2.gyp:tripod_core',
    ],
    'sources' : [
      
      #### Cpp Files Generated from ICE ##################### 
      '<(oce_slice_path)/ILikeIt.cpp',
      '<(oce_slice_path)/Notify.cpp',

      #### Adapter Cpp Files ################################ 
#      '<(adapter_path)/i_like_it_cache_tripod_adapter.cc'
    ],
  }, 



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

  ######################################################
  {
    'target_name' : '<(service_name)Test',
    'type' : 'executable',
    'dependencies' : [
      '../gtest.gyp:gtest',
      '../gtest.gyp:gtest_main',
      '../3rdparty.gyp:hiredis',
      '../base.gyp:base',
      '../xce_base.gyp:xce_base',
      '<(service_name)_oce_slice_and_adapter',
      '../tripod2.gyp:tripod_core',
    ],
    'sources' : [
      '<(oce_proto_path)/Like.pb.cc',
      '<(adapter_path)/i_like_it_cache_tripod_adapter.cc',
      '<(adapter_path)/test/like_tripod_adapter_test.cc',
    ],
  }, 

  ######################################################
  {
    'target_name' : 'ILikeItLogic',
    'type' : 'shared_library',
    'dependencies' : [
      '../xce_base.gyp:xce_base',
    ],

    'sources' : [
      '<(service_src_path)/logic/ILikeItManagerI.cpp',
      '<(adapter_path)/ILikeItLogicAdapter.cpp',
      '<(adapter_path)/ILikeItCacheAdapter.cpp',
      '<(adapter_path)/UserCacheAdapter.cpp',
      '<(adapter_path)/DistUserCacheReloaderAdapter.cpp',
      '<(oce_slice_path)/ILikeIt.cpp',
      '<(oce_slice_path)/DataSync.cpp',
      '<(oce_slice_path)/UserBase.cpp',
      '<(oce_slice_path)/Notify.cpp',
      '<(oce_slice_path)/UserCache.cpp',
      '<(oce_proto_path)/DataSync.pb.cc',
      '<(oce_proto_path)/UserCache.pb.cc',
    ],
  }, 

  ######################################################
  {
    'target_name' : 'ILikeItCache',
    'type' : 'shared_library',
    'dependencies' : [
      '../xce_base.gyp:xce_base',
    ],
    'sources' : [
      '<(service_src_path)/cache/ILikeItCacheI.cpp',
      '<(adapter_path)/ILikeItCacheAdapter.cpp',
      '<(adapter_path)/UserCacheAdapter.cpp',
      '<(adapter_path)/DistUserCacheReloaderAdapter.cpp',
      '<(adapter_path)/BuddyByIdReplicaAdapter.cpp',
      '<(oce_slice_path)/ILikeIt.cpp',
      '<(oce_slice_path)/BuddyByIdCache.cpp',
      '<(oce_slice_path)/DataSync.cpp',
      '<(oce_slice_path)/UserBase.cpp',
      '<(oce_slice_path)/Notify.cpp',
      '<(oce_slice_path)/UserCache.cpp',
      '<(oce_proto_path)/DataSync.pb.cc',
      '<(oce_proto_path)/UserCache.pb.cc',
    ],
  }, 

  ######################################################
  {
    'target_name' : 'ILikeItPreloader',
    'type' : 'shared_library',
    'dependencies' : [
      '../xce_base.gyp:xce_base',
    ],
    'sources' : [
      '<(service_src_path)/preloader/cpp/ILikeItPreloader.cpp',
      '<(adapter_path)/ILikeItCacheAdapter.cpp',
      '<(adapter_path)/UserCacheAdapter.cpp',
      '<(adapter_path)/DistUserCacheReloaderAdapter.cpp',
      '<(adapter_path)/BuddyByIdReplicaAdapter.cpp',
      '<(oce_slice_path)/ILikeIt.cpp',
      '<(oce_slice_path)/BuddyByIdCache.cpp',
      '<(oce_slice_path)/UserBase.cpp',
      '<(oce_slice_path)/Notify.cpp',
      '<(oce_slice_path)/UserCache.cpp',
      '<(oce_proto_path)/UserCache.pb.cc',
    ],
  }, 

  ######################################################
  {
    'target_name' : 'TripodLikeCache',
    'type' : 'shared_library',
    'dependencies' : [
#      '../xce_base.gyp:xce_base',
      '../tripod.gyp:tripod_base',
    ],
    'sources' : [
      '<(service_src_path)/tripodcache/TripodLikeCache.cpp',
      '<(adapter_path)/ILikeItCacheAdapter.cpp',
      '<(adapter_path)/UserCacheAdapter.cpp',
      '<(adapter_path)/DistUserCacheReloaderAdapter.cpp',
      '<(adapter_path)/BuddyByIdReplicaAdapter.cpp',
      '<(oce_slice_path)/BuddyByIdCache.cpp',
      '<(oce_slice_path)/UserBase.cpp',
      '<(oce_slice_path)/UserCache.cpp',
      '<(oce_proto_path)/DataSync.pb.cc',
      '<(oce_proto_path)/UserCache.pb.cc',
    ],
  }, 


  ] #end targets
}

