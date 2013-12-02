{
  #包含公共设置
  'includes':[
    'mini_group_common.gyp',
  ],
  'variables':{
    'service_name'            : 'MiniGroup',

    'service_src_path'        : '<(main_project_path)/app/MiniGroup',
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
    'target_name' : 'MiniGroupManager',
    'type' : 'shared_library',
    'dependencies' : [
      '../xce_base.gyp:xce_base',
    ],

    'sources' : [
      '<(service_src_path)/MiniGroupManager/src/MiniGroupManagerI.cpp',
      '<(service_src_path)/MiniGroupCommon/src/MiniGroupCommon.cpp',
      '<(adapter_path)/MiniGroupWriterAdapter.cpp',
      '<(adapter_path)/BatchUpdaterAdapter.cpp',
      '<(adapter_path)/MonitorAdapter.cpp',
      '<(oce_slice_path)/MiniGroup.cpp',
      '<(oce_slice_path)/BatchUpdater.cpp',
      '<(oce_slice_path)/Monitor.cpp',
      '<(oce_proto_path)/MiniGroup.pb.cc',
      '<(oce_proto_path)/User2MiniGroup.pb.cc',
    ],
  }, 

  ######################################################
  {
    'target_name' : 'MiniGroupWriter',
    'type' : 'shared_library',
    'dependencies' : [
      '../xce_base.gyp:xce_base',
    ],

    'sources' : [
      '<(service_src_path)/MiniGroupWriter/src/MiniGroupWriterI.cpp',
      '<(service_src_path)/MiniGroupCommon/src/MiniGroupCommon.cpp',
      '<(adapter_path)/MiniGroupManagerAdapter.cpp',
      '<(adapter_path)/BatchUpdaterAdapter.cpp',
      '<(adapter_path)/MonitorAdapter.cpp',
      '<(oce_slice_path)/MiniGroup.cpp',
      '<(oce_slice_path)/BatchUpdater.cpp',
      '<(oce_slice_path)/Monitor.cpp',
      '<(oce_slice_path)/DataSync.cpp',
      '<(oce_proto_path)/MiniGroup.pb.cc',
      '<(oce_proto_path)/DataSync.pb.cc',
      '<(oce_proto_path)/User2MiniGroup.pb.cc',
    ],
  }, 

  ######################################################
  {
    'target_name' : 'MiniGroupPreloader',
    'type' : 'shared_library',
    'dependencies' : [
      '../xce_base.gyp:xce_base',
    ],

    'sources' : [
      '<(service_src_path)/MiniGroupPreloader/src/MiniGroupPreloader.cpp',
      '<(service_src_path)/MiniGroupCommon/src/MiniGroupCommon.cpp',
      '<(adapter_path)/MiniGroupManagerAdapter.cpp',
      '<(adapter_path)/BatchUpdaterAdapter.cpp',
      '<(adapter_path)/MonitorAdapter.cpp',
      '<(oce_slice_path)/MiniGroup.cpp',
      '<(oce_slice_path)/BatchUpdater.cpp',
      '<(oce_slice_path)/Monitor.cpp',
      '<(oce_proto_path)/MiniGroup.pb.cc',
      '<(oce_proto_path)/User2MiniGroup.pb.cc',
    ],
  }, 

  ######################################################
  {
    'target_name' : 'User2MiniGroupPreloader',
    'type' : 'shared_library',
    'dependencies' : [
      '../xce_base.gyp:xce_base',
    ],

    'sources' : [
      '<(service_src_path)/User2MiniGroupPreloader/src/User2MiniGroupPreloader.cpp',
      '<(service_src_path)/MiniGroupCommon/src/MiniGroupCommon.cpp',
      '<(adapter_path)/MiniGroupManagerAdapter.cpp',
      '<(adapter_path)/BatchUpdaterAdapter.cpp',
      '<(adapter_path)/MonitorAdapter.cpp',
      '<(oce_slice_path)/MiniGroup.cpp',
      '<(oce_slice_path)/BatchUpdater.cpp',
      '<(oce_slice_path)/Monitor.cpp',
      '<(oce_proto_path)/MiniGroup.pb.cc',
      '<(oce_proto_path)/User2MiniGroup.pb.cc',
    ],
  }, 

  ######################################################
  {
    'target_name' : 'MiniGroupCommon',
    'type' : 'static_library',
    'dependencies' : [
      '../xce_base.gyp:xce_base',
    ],

    'sources' : [
      '<(service_src_path)/MiniGroupCommon/src/MiniGroupCommon.cpp',
      '<(adapter_path)/BatchUpdaterAdapter.cpp',
      '<(adapter_path)/MonitorAdapter.cpp',
      '<(oce_slice_path)/MiniGroup.cpp',
      '<(oce_slice_path)/BatchUpdater.cpp',
      '<(oce_slice_path)/Monitor.cpp',
      '<(oce_proto_path)/MiniGroup.pb.cc',
      '<(oce_proto_path)/User2MiniGroup.pb.cc',
    ],
  }, 

  ] #end targets
}

