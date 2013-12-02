{
  #包含公共设置
  'includes':[
    'bulletin_common.gyp',
  ],
  'variables':{
    'service_name'            : 'Bulletin',

    'service_src_path'        : '<(main_project_path)/app/Bulletin',
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
    'target_name' : 'BulletinAdmin',
    'type' : 'shared_library',
    'dependencies' : [
      '../xce_base.gyp:xce_base',
    ],

    'sources' : [
      '<(service_src_path)/admin/BulletinAdminI.cpp',
      '<(adapter_path)/BulletinAdapter.cpp',
      '<(oce_slice_path)/Bulletin.cpp',
    ],
  }, 

  ######################################################
  {
    'target_name' : 'BulletinManager',
    'type' : 'shared_library',
    'dependencies' : [
      '../xce_base.gyp:xce_base',
    ],

    'sources' : [
      '<(service_src_path)/manager/BulletinManagerI.cpp',
      '<(adapter_path)/BulletinAdapter.cpp',
      '<(adapter_path)/UserCacheAdapter.cpp',
      '<(adapter_path)/DistUserCacheReloaderAdapter.cpp',
      '<(adapter_path)/UserBornAdapter.cpp',
      '<(adapter_path)/UserStageAdapter.cpp',
      '<(adapter_path)/UserDescAdapter.cpp',
      '<(adapter_path)/UserDescReaderAdapter.cpp',
      '<(adapter_path)/UserNetworkAdapter.cpp',
      '<(adapter_path)/UserNetworkReaderAdapter.cpp',
      '<(adapter_path)/UserNetworkWriterAdapter.cpp',
      '<(oce_slice_path)/Bulletin.cpp',
      '<(oce_slice_path)/UserCache.cpp',
      '<(oce_slice_path)/UserBase.cpp',
      '<(oce_slice_path)/UserDesc.cpp',
      '<(oce_slice_path)/UserNetwork.cpp',
      '<(oce_proto_path)/UserCache.pb.cc',
    ],
  }, 

  ] #end targets
}

