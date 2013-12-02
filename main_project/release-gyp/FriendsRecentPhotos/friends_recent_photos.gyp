{
  #包含公共设置
  'includes':[
    'friends_recent_photos_common.gyp',
  ],
  'variables':{
    'service_name'            : 'FriendsRecentPhotos',

    'service_src_path'        : '<(main_project_path)/app/FriendsRecentPhotos',
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
    'target_name' : 'FriendsRecentPhotosManager',
    'type' : 'shared_library',
    'dependencies' : [
      '../xce_base.gyp:xce_base',
    ],

    'sources' : [
      '<(service_src_path)/FriendsRecentPhotosManager/src/FriendsRecentPhotosManagerI.cpp',
      '<(adapter_path)/FriendsRecentPhotosWriterAdapter.cpp',
      '<(adapter_path)/BuddyByIdReplicaAdapter.cpp',
      '<(adapter_path)/BatchUpdaterAdapter.cpp',
      '<(adapter_path)/MonitorAdapter.cpp',
      '<(oce_slice_path)/FriendsRecentPhotos.cpp',
      '<(oce_slice_path)/BatchUpdater.cpp',
      '<(oce_slice_path)/BuddyByIdCache.cpp',
      '<(oce_slice_path)/Monitor.cpp',
    ],
  }, 

  ######################################################
  {
    'target_name' : 'FriendsRecentPhotosWriter',
    'type' : 'shared_library',
    'dependencies' : [
      '../xce_base.gyp:xce_base',
    ],

    'sources' : [
      '<(service_src_path)/FriendsRecentPhotosWriter/src/FriendsRecentPhotosWriterI.cpp',
      '<(adapter_path)/FriendsRecentPhotosManagerAdapter.cpp',
      '<(adapter_path)/BatchUpdaterAdapter.cpp',
      '<(adapter_path)/MonitorAdapter.cpp',
      '<(oce_slice_path)/FriendsRecentPhotos.cpp',
      '<(oce_slice_path)/BatchUpdater.cpp',
      '<(oce_slice_path)/Monitor.cpp',
      '<(oce_slice_path)/MiniGroup.cpp',
    ],
  }, 

  ######################################################
  {
    'target_name' : 'FriendsRecentPhotosPreloader',
    'type' : 'shared_library',
    'dependencies' : [
      '../xce_base.gyp:xce_base',
    ],

    'sources' : [
      '<(service_src_path)/FriendsRecentPhotosPreloader/src/FriendsRecentPhotosPreloader.cpp',
      '<(service_src_path)/../MiniGroup/MiniGroupCommon/src/MiniGroupCommon.cpp',
      '<(adapter_path)/BatchUpdaterAdapter.cpp',
      '<(adapter_path)/MonitorAdapter.cpp',
      '<(oce_slice_path)/FriendsRecentPhotos.cpp',
      '<(oce_slice_path)/BatchUpdater.cpp',
      '<(oce_slice_path)/Monitor.cpp',
      '<(oce_slice_path)/MiniGroup.cpp',
      '<(oce_proto_path)/MiniGroup.pb.cc',
      '<(oce_proto_path)/User2MiniGroup.pb.cc',
    ],
  }, 

  ######################################################
  {
    'target_name' : 'FriendsPhotosDBOperator',
    'type' : 'executable',
    'dependencies' : [
      '../xce_base.gyp:xce_base',
    ],

    'sources' : [
      '<(service_src_path)/tools/src/main.cpp',
      '<(service_src_path)/../MiniGroup/MiniGroupCommon/src/MiniGroupCommon.cpp',
      '<(adapter_path)/BatchUpdaterAdapter.cpp',
      '<(adapter_path)/MonitorAdapter.cpp',
      '<(adapter_path)/BatchUpdaterAdapter.cpp',
      '<(adapter_path)/BuddyCoreAdapter.cpp',
      '<(oce_slice_path)/FriendsRecentPhotos.cpp',
      '<(oce_slice_path)/BatchUpdater.cpp',
      '<(oce_slice_path)/Monitor.cpp',
      '<(oce_slice_path)/MiniGroup.cpp',
      '<(oce_slice_path)/BuddyByIdCache.cpp',
      '<(oce_slice_path)/BuddyCore.cpp',
      '<(oce_proto_path)/MiniGroup.pb.cc',
      '<(oce_proto_path)/User2MiniGroup.pb.cc',
    ],
  }, 



  ] #end targets
}

