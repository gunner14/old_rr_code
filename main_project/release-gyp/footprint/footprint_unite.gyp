{
  #包含公共设置
  'includes':[
    'footprint_common.gyp',
  ],
  'variables':{
    'service_name'            : 'FootprintUnite',

    'service_src_path'        : '<(main_project_path)/app/FootprintUnite',
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
    'target_name' : 'FootprintFacade',
    'type' : 'shared_library',
    'dependencies' : [
      '../xce_base.gyp:xce_base',
    ],

    'sources' : [
      '<(service_src_path)/FootprintFacade/FootprintFacadeI.cpp',
      '<(adapter_path)/LoveFootprintByIntervieweeAdapter.cpp',
      '<(adapter_path)/LoveFootprintByInterviewerAdapter.cpp',
      '<(adapter_path)/VisitFootprintAdapter.cpp',
      '<(adapter_path)/HomeFootprintAdapter.cpp',
      '<(adapter_path)/FootprintFacadeAdapter.cpp',
      '<(adapter_path)/MonitorAdapter.cpp',
      '<(oce_slice_path)/FootprintUnite.cpp',
      '<(oce_slice_path)/BatchUpdater.cpp',
      '<(oce_slice_path)/Monitor.cpp',
    ],
  }, 

  ######################################################
  {
    'target_name' : 'HomeFootprint',
    'type' : 'shared_library',
    'dependencies' : [
      '../xce_base.gyp:xce_base',
    ],

    'sources' : [
      '<(service_src_path)/HomeFootprint/HomeFootprintCacheI.cpp',
      '<(adapter_path)/VisitFootprintAdapter.cpp',
      '<(adapter_path)/HomeFootprintAdapter.cpp',
      '<(adapter_path)/FootprintFacadeAdapter.cpp',
      '<(adapter_path)/MonitorAdapter.cpp',
      '<(adapter_path)/FootprintViewAdapter.cpp',
      '<(adapter_path)/BatchUpdaterAdapter.cpp',
      '<(adapter_path)/HomeFootprintReplicaAdapter.cpp',
      '<(oce_slice_path)/FootprintUnite.cpp',
      '<(oce_slice_path)/BatchUpdater.cpp',
      '<(oce_slice_path)/Monitor.cpp',
      '<(oce_slice_path)/DataSync.cpp',
      '<(oce_slice_path)/FootprintView.cpp',
      '<(oce_proto_path)/DataSync.pb.cc',
    ],
  }, 

  ######################################################
  {
    'target_name' : 'FootprintView',
    'type' : 'shared_library',
    'dependencies' : [
      '../xce_base.gyp:xce_base',
    ],

    'sources' : [
      '<(service_src_path)/FootprintView/src/FootprintViewI.cpp',
      '<(adapter_path)/VisitFootprintAdapter.cpp',
      '<(adapter_path)/HomeFootprintAdapter.cpp',
      '<(adapter_path)/MonitorAdapter.cpp',
      '<(adapter_path)/FootprintFacadeAdapter.cpp',
      '<(adapter_path)/FootprintViewAdapter.cpp',
      '<(oce_slice_path)/FootprintUnite.cpp',
      '<(oce_slice_path)/BatchUpdater.cpp',
      '<(oce_slice_path)/Monitor.cpp',
      '<(oce_slice_path)/FootprintView.cpp',
    ],
  }, 

  ######################################################
  { 
    'target_name' : 'VisitFootprint',
    'type' : 'shared_library',
    'dependencies' : [
      '../xce_base.gyp:xce_base',
    ],

    'sources' : [
      '<(service_src_path)/UgcFootprint/UgcFootprintCacheI.cpp',
      '<(adapter_path)/VisitFootprintAdapter.cpp',
      '<(adapter_path)/MonitorAdapter.cpp',
      '<(adapter_path)/BatchUpdaterAdapter.cpp',
      '<(adapter_path)/FootprintFacadeAdapter.cpp',
      '<(adapter_path)/HomeFootprintReplicaAdapter.cpp',
      '<(oce_slice_path)/FootprintUnite.cpp',
      '<(oce_slice_path)/BatchUpdater.cpp',
      '<(oce_slice_path)/Monitor.cpp',
    ],
  }, 

  ######################################################
  {
    'target_name' : 'LoveFootprint',
    'type' : 'shared_library',
    'dependencies' : [
      '../xce_base.gyp:xce_base',
    ],

    'sources' : [
      '<(service_src_path)/CommonFootprint/CommonFootprintCacheI.cpp',
      '<(adapter_path)/MonitorAdapter.cpp',
      '<(adapter_path)/HomeFootprintReplicaAdapter.cpp',
      '<(oce_slice_path)/FootprintUnite.cpp',
      '<(oce_slice_path)/BatchUpdater.cpp',
      '<(oce_slice_path)/Monitor.cpp',
    ],
  }, 

  ######################################################
  {
    'target_name' : 'HomeFootprintReaderPreloader',
    'type' : 'shared_library',
    'dependencies' : [
      '../xce_base.gyp:xce_base',
    ],

    'sources' : [
      '<(service_src_path)/Preloader/HomeFootprintPreloader/HomeFootprintReaderPreloader/HomeFootprintReaderPreloader.cpp',
      '<(adapter_path)/VisitFootprintAdapter.cpp',
      '<(adapter_path)/BatchUpdaterAdapter.cpp',
      '<(adapter_path)/HomeFootprintAdapter.cpp',
      '<(adapter_path)/MonitorAdapter.cpp',
      '<(adapter_path)/FootprintFacadeAdapter.cpp',
      '<(oce_slice_path)/FootprintUnite.cpp',
      '<(oce_slice_path)/BatchUpdater.cpp',
      '<(oce_slice_path)/Monitor.cpp',
      '<(oce_slice_path)/FootprintView.cpp',
    ],
  }, 

  ######################################################
  {
    'target_name' : 'HomeFootprintBufReaderPreloader',
    'type' : 'shared_library',
    'dependencies' : [
      '../xce_base.gyp:xce_base',
    ],

    'sources' : [
      '<(service_src_path)/Preloader/HomeFootprintPreloader/HomeFootprintBufReaderPreloader/HomeFootprintBufReaderPreloader.cpp',
      '<(adapter_path)/VisitFootprintAdapter.cpp',
      '<(adapter_path)/BatchUpdaterAdapter.cpp',
      '<(adapter_path)/HomeFootprintAdapter.cpp',
      '<(adapter_path)/MonitorAdapter.cpp',
      '<(adapter_path)/FootprintFacadeAdapter.cpp',
      '<(oce_slice_path)/FootprintUnite.cpp',
      '<(oce_slice_path)/BatchUpdater.cpp',
      '<(oce_slice_path)/Monitor.cpp',
      '<(oce_slice_path)/FootprintView.cpp',
    ],
  }, 

  ######################################################
  {
    'target_name' : 'HomeUserVisitCountReaderPreloader',
    'type' : 'shared_library',
    'dependencies' : [
      '../xce_base.gyp:xce_base',
    ],

    'sources' : [
      '<(service_src_path)/Preloader/HomeFootprintPreloader/HomeUserVisitCountReaderPreloader/HomeUserVisitCountReaderPreloader.cpp',
      '<(adapter_path)/VisitFootprintAdapter.cpp',
      '<(adapter_path)/BatchUpdaterAdapter.cpp',
      '<(adapter_path)/HomeFootprintAdapter.cpp',
      '<(adapter_path)/MonitorAdapter.cpp',
      '<(adapter_path)/FootprintFacadeAdapter.cpp',
      '<(oce_slice_path)/FootprintUnite.cpp',
      '<(oce_slice_path)/BatchUpdater.cpp',
      '<(oce_slice_path)/Monitor.cpp',
      '<(oce_slice_path)/FootprintView.cpp',
    ],
  }, 

  ######################################################
  {
    'target_name' : 'HomeFootprintReplicaReaderPreloader',
    'type' : 'shared_library',
    'dependencies' : [
      '../xce_base.gyp:xce_base',
    ],

    'sources' : [
      '<(service_src_path)/Preloader/HomeFootprintReplicaPreloader/HomeFootprintReplicaReaderPreloader/HomeFootprintReplicaReaderPreloader.cpp',
      '<(adapter_path)/VisitFootprintAdapter.cpp',
      '<(adapter_path)/BatchUpdaterAdapter.cpp',
      '<(adapter_path)/HomeFootprintAdapter.cpp',
      '<(adapter_path)/MonitorAdapter.cpp',
      '<(adapter_path)/FootprintFacadeAdapter.cpp',
      '<(oce_slice_path)/FootprintUnite.cpp',
      '<(oce_slice_path)/BatchUpdater.cpp',
      '<(oce_slice_path)/Monitor.cpp',
      '<(oce_slice_path)/FootprintView.cpp',
    ],
  }, 

  ######################################################
  {
    'target_name' : 'HomeFootprintBufReplicaReaderPreloader',
    'type' : 'shared_library',
    'dependencies' : [
      '../xce_base.gyp:xce_base',
    ],

    'sources' : [
      '<(service_src_path)/Preloader/HomeFootprintReplicaPreloader/HomeFootprintBufReplicaReaderPreloader/HomeFootprintBufReplicaReaderPreloader.cpp',
      '<(adapter_path)/VisitFootprintAdapter.cpp',
      '<(adapter_path)/BatchUpdaterAdapter.cpp',
      '<(adapter_path)/HomeFootprintAdapter.cpp',
      '<(adapter_path)/MonitorAdapter.cpp',
      '<(adapter_path)/FootprintFacadeAdapter.cpp',
      '<(oce_slice_path)/FootprintUnite.cpp',
      '<(oce_slice_path)/BatchUpdater.cpp',
      '<(oce_slice_path)/Monitor.cpp',
      '<(oce_slice_path)/FootprintView.cpp',
    ],
  }, 

  ######################################################
  {
    'target_name' : 'HomeUserVisitCountReplicaReaderPreloader',
    'type' : 'shared_library',
    'dependencies' : [
      '../xce_base.gyp:xce_base',
    ],

    'sources' : [
      '<(service_src_path)/Preloader/HomeFootprintReplicaPreloader/HomeUserVisitCountReplicaReaderPreloader/HomeUserVisitCountReplicaReaderPreloader.cpp',
      '<(adapter_path)/VisitFootprintAdapter.cpp',
      '<(adapter_path)/BatchUpdaterAdapter.cpp',
      '<(adapter_path)/HomeFootprintAdapter.cpp',
      '<(adapter_path)/MonitorAdapter.cpp',
      '<(adapter_path)/FootprintFacadeAdapter.cpp',
      '<(oce_slice_path)/FootprintUnite.cpp',
      '<(oce_slice_path)/BatchUpdater.cpp',
      '<(oce_slice_path)/Monitor.cpp',
      '<(oce_slice_path)/FootprintView.cpp',
    ],
  }, 

  ######################################################
  {
    'target_name' : 'HomeFootprintReplica',
    'type' : 'shared_library',
    'dependencies' : [
      '../xce_base.gyp:xce_base',
    ],

    'sources' : [
      '<(service_src_path)/HomeFootprintReplica/HomeFootprintReplicaI.cpp',
      '<(adapter_path)/VisitFootprintAdapter.cpp',
      '<(adapter_path)/BatchUpdaterAdapter.cpp',
      '<(adapter_path)/HomeFootprintAdapter.cpp',
      '<(adapter_path)/MonitorAdapter.cpp',
      '<(adapter_path)/FootprintFacadeAdapter.cpp',
      '<(oce_slice_path)/FootprintUnite.cpp',
      '<(oce_slice_path)/BatchUpdater.cpp',
      '<(oce_slice_path)/Monitor.cpp',
      '<(oce_slice_path)/FootprintView.cpp',
    ],
  }, 

  ######################################################
  {
    'target_name' : 'FootprintTool',
    'type' : 'executable',
    'dependencies' : [
      '../xce_base.gyp:xce_base',
    ],

    'sources' : [
      '<(service_src_path)/tools/HomeFootprintDBOperator.cpp',
      '<(service_src_path)/tools/main.cpp',
      '<(adapter_path)/MonitorAdapter.cpp',
      '<(oce_slice_path)/FootprintUnite.cpp',
      '<(oce_slice_path)/FootprintView.cpp',
      '<(oce_slice_path)/BatchUpdater.cpp',
      '<(oce_slice_path)/Monitor.cpp',
    ],
  }, 


  ] #end targets
}

