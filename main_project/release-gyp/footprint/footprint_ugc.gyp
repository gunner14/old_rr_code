{
  #包含公共设置
  'includes':[
    'footprint_common.gyp',
  ],
  'variables':{
    'service_name'            : 'FootprintUgc',

    'service_src_path'        : '<(main_project_path)/app/FootprintUgc/src',
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
    'target_name' : 'BlogFootprintL',
    'type' : 'shared_library',
    'dependencies' : [
      '../xce_base.gyp:xce_base',
    ],

    'sources' : [
      '<(service_src_path)/FootprintI.cpp',
      '<(adapter_path)/MonitorAdapter.cpp',
      '<(adapter_path)/BatchUpdaterAdapter.cpp',
      '<(oce_slice_path)/Footprint.cpp',
      '<(oce_slice_path)/FootprintNew.cpp',
      '<(oce_slice_path)/FootprintFilter.cpp',
      '<(oce_slice_path)/BatchUpdater.cpp',
      '<(oce_slice_path)/Monitor.cpp',
    ],
  }, 

 
  ] #end targets
}

