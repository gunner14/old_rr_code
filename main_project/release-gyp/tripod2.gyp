{
  'includes':[
    'global_vars.gyp',
  ],
  'conditions':[
    ['OS=="linux"', {
      'target_defaults': {
        'cflags':['-fPIC', '-g', '-O2'],
        'defines':['OS_LINUX', 'POSIX', 'NEWARCH'],
        'include_dirs':[
          '..',
          '<(ice_path)/include',
          '/usr/include/mysql',
          '/usr/local/xce/include',
          '/usr/local/distcache/include', #ugly!
          '/usr/local/distcache-dev/include', #ugly!
          '<(boost_path)/include/boost-1_38',
          '../UtilCxx/src',
          '../IceExt/src',
          '../util/cpp',
          '../DbCxxPool/src',
          '../UtilSlice/slice',
          '../OceSlice/slice',
          '../OceProto/proto',
          '../OceCxxAdapter/src',
          '../third-party/redis-c-driver',
          '../third-party/google-breakpad/src/',
        ],
        'link_settings' : {
          'ldflags': [
            '-Wl,-rpath <(boost_path)/lib',
            '-Wl,-rpath <(ice_path)/lib64',
            '-Wl,-rpath /usr/local/xce-dev/lib64',
            '-Wl,-rpath /usr/local/distcache/lib',
            '-Wl,-rpath /usr/local/xce/lib/',
          ],
          'libraries' : [
            '-L<(ice_path)/lib64',
            '-lIce', '-lIceXML', '-lIceUtil', '-lIceStorm', '-lIceBox', '-lIceGrid', 
            '-L<(boost_path)/lib',
            '-lboost_date_time-gcc41-mt-1_38', '-lboost_system-gcc41-mt-1_38', '-lboost_regex-gcc41-mt-1_38', 
            '-lboost_thread-gcc41-mt-1_38', '-lboost_filesystem-gcc41-mt-1_38',
            '-L/usr/local/xce/lib64',
            '-lmysqlpp', 
            '-llog4cplus',
            '-L/usr/lib64/mysql',
            '-lmysqlclient_r',
            '-licudata', '-licui18n', '-licuuc',
            '-L/usr/local/distcache/lib',
            '-lmemcached', 
            '-L/usr/local/distcache-dev/lib',
            '-lprotobuf', '-lzookeeper_mt',
            '-lstdc++', '-lm', '-lrt','-lpthread',
            '-L/usr/local/xce-dev/lib',
          ],
        },
        'dependencies' : [
          'oce_slice.gyp:oce_slice',
          'oce_proto.gyp:oce_proto',
          'xce_base.gyp:util_slice_ice',
          '3rdparty.gyp:breakpad',
        ],
      },
     },
    ],
    ['OS=="win"', 
      {'target_defaults': {},},
    ],
  ], # end conditions

  # proto and slice files
  'targets' : [
  {
    'target_name' : 'tripod_slice_and_proto',
    'type' : 'static_library',
    'defines' : [],
    'sources' : [
      '../OceSlice/slice/tripod2.cpp',
      '../OceProto/proto/HomeFootprintSet.pb.cc',
      '../OceProto/proto/Like.pb.cc',
      '../OceProto/proto/listorhashvalue.pb.cc',
      '../OceProto/proto/MenuTripodData.pb.cc',
      '../OceProto/proto/tripod_options.pb.cc',
      '../OceProto/proto/UserBase.pb.cc',
      '../OceProto/proto/UserConfig.pb.cc',
      '../OceProto/proto/UserCount.pb.cc',
      '../OceProto/proto/UserDesc.pb.cc',
      '../OceProto/proto/user_network.pb.cc',
      '../OceProto/proto/user_score.pb.cc',
      '../OceProto/proto/UserViewCount.pb.cc',
      '../OceProto/proto/vip_info.pb.cc',
      '../OceProto/proto/account_connect.pb.cc',
      '../OceProto/proto/Ticket.pb.cc',
    ],
  },
  {
    'target_name' : 'tripod_core',
    'type' : 'static_library',
    'defines' : [],
    'dependencies' : [
      '3rdparty.gyp:hiredis',
      'base.gyp:base',
      'tripod_slice_and_proto',
    ],
    'sources' : [
      '../tripod2/cc/core/singleton.h',
      '../tripod2/cc/core/zk_client.cc',
      '../tripod2/cc/core/common.cc',
      '../tripod2/cc/core/cache_client.cc',
      '../tripod2/cc/core/operation_invoke_statistics.cc',
      '../tripod2/cc/core/invoke_client.cc',
      '../tripod2/cc/core/redis_client.cc',
      '../tripod2/cc/core/tripod_client.cc',
      '../tripod2/cc/core/remote_logger.cc',
      '../tripod2/cc/core/cache_proxy.cc',
      '../tripod2/cc/core/static_cache_proxy.cc',
      '../tripod2/cc/core/cache_manager_proxy.cc',
      '../tripod2/cc/core/producer.cc',
      '../tripod2/cc/core/producer_manager_proxy.cc',
      '../tripod2/cc/core/protobuf_reflection_util.cc',
      '../tripod2/cc/core/single_table_tripod_client.cc',
      '../tripod2/cc/core/multibiz_tripod_client.cc',
      '../tripod2/cc/core/multibiz_cache_proxy.cc',
      '../tripod2/cc/core/bucket_lock.cc',
    ],
  },
  {
    'target_name' : 'tripod_core_test',
    'type' : 'executable',
    'defines' : [],
    'dependencies' : [
      'tripod_core',
      'gtest.gyp:gtest',
      'gtest.gyp:gtest_main',
      'xce_base.gyp:xce_base',
    ],
    'sources' : [
      #'../tripod2/cc/core/test/hello_test.cc',
      #'../tripod2/cc/core/test/zk_client_test.cc',
      #'../tripod2/cc/core/test/redis_client_test.cc',
      #'../tripod2/cc/core/test/redis_client_pool_test.cc',
      #'../tripod2/cc/core/test/cache_proxy_test.cc',
      #'../tripod2/cc/core/test/cache_manager_proxy_test.cc',
      #'../tripod2/cc/core/test/producer_manager_proxy_test.cc',
      '../tripod2/cc/core/test/tripod_client_test.cc',
      #'../tripod2/cc/core/test/single_table_tripod_client_test.cc',
      #'../tripod2/cc/core/test/bucket_lock_test.cc',
    ],
  },
  {
    'target_name' : 'tripod_producer_manager_test',
    'type' : 'executable',
    'defines' : [],
    'dependencies' : [
      'tripod_core',
      'gtest.gyp:gtest',
      'gtest.gyp:gtest_main',
      'xce_base.gyp:xce_base',
    ],
    'sources' : [
      '../tripod2/cc/core/test/producer_manager_proxy_test.cc',
    ],
  },
  {
    'target_name' : 'tripod_cache_manager_test',
    'type' : 'executable',
    'defines' : [],
    'dependencies' : [
      'tripod_core',
      'gtest.gyp:gtest',
      'gtest.gyp:gtest_main',
      'xce_base.gyp:xce_base',
    ],
    'sources' : [
      '../tripod2/cc/core/test/cache_manager_proxy_test.cc',
    ],
  },
  {
    'target_name' : 'tripod_zk_admin',
    'type' : 'executable',
    'defines' : [],
    'dependencies' : [
      'tripod_core', 
    ],
    'sources' : [
      '../tripod2/cc/admin/xml_node.cc',
      '../tripod2/cc/admin/tripod_config.cc',
      '../tripod2/cc/admin/main.cc',
    ],
  },
  {
    'target_name' : 'tripodcachemanager',
    'type' : 'shared_library',
    'defines' : [],
    'dependencies' : [
      'tripod_core',
      'xce_common.gyp:xce_common',
      'xce_base.gyp:xce_base',
      'xce_base.gyp:db_cxx_pool',
    ],
    'sources' : [
      '../tripod2/cc/cachemanager/cache_manager_impl.cc',
      #'../OceProto/proto/listorhashvalue.pb.cc',
    ],
  },
  {
    'target_name' : 'tripodproducer_slice',
    'type' : 'static_library',
    'defines' : [],
    'dependencies' : [
    ],
    'sources' : [
      '../OceSlice/slice/FootprintUnite.cpp',
      '../OceSlice/slice/ILikeIt.cpp',
      '../OceSlice/slice/MenuCache.cpp',
      '../OceSlice/slice/Notify.cpp',
    ],
  },
  # producermanager和batch_checke都依赖该目标 
  {
    'target_name' : 'tripodproducer',
    'type' : 'static_library',
    'defines' : [],
    'dependencies' : [
      'tripod_core',
      'tripodproducer_slice',
      'xce_base.gyp:xce_base',
      'xce_base.gyp:db_cxx_pool',
    ],
    'sources' : [
      '../tripod2/cc/producermanager/producer_factory.cc',
      '../tripod2/cc/producermanager/example_producer.cc',
      '../tripod2/cc/producermanager/single_table_producer.cc',
      '../tripod2/cc/producermanager/simple_counter_producer.cc',
      #'../tripod2/cc/producermanager/user_count_producer.cc',
      '../tripod2/cc/producermanager/user_score_vip_producer.cc',
      '../tripod2/cc/producermanager/user_score_login_producer.cc',
      '../tripod2/cc/producermanager/home_foot_print_producer.cc',
      '../tripod2/cc/producermanager/user_view_count_producer.cc',
      '../app/MenuCache/MenuTripod/MenuTripodShare.cpp',
      '../tripod2/cc/producermanager/menu_producer.cc',
      '../tripod2/cc/producermanager/like_producer.cc',
      '../tripod2/cc/producermanager/account_connect_producer.cc',
      '../tripod2/cc/producermanager/ticket_producer.cc',
    ],
  },
  {
    'target_name' : 'batch_checker',
    'type' : 'executable',
    'dependencies' : [ 
      'tripodproducer',
    ], 
    'sources' : [ 
      '<(main_project_path)/tripod2/cc/checker/batchchecker/batch_checker.cc',
      '<(main_project_path)/tripod2/cc/checker/batchchecker/old_checker.cc',
      '<(main_project_path)/tripod2/cc/checker/batchchecker/config_handler.cc',
      '<(main_project_path)/tripod2/cc/checker/batchchecker/cache_handler.cc',
      '<(main_project_path)/tripod2/cc/checker/batchchecker/pc_impl.cc',
      #'<(main_project_path)/tripod2/cc/producermanager/producer_factory.cc',
      '<(main_project_path)/OceSlice/slice/UserBase.cpp',

      #old checker sucks
      '<(main_project_path)/OceCxxAdapter/src/UserBornAdapter.cpp',
      '<(main_project_path)/OceCxxAdapter/src/UserDoingAdapter.cpp',
      '<(main_project_path)/OceCxxAdapter/src/UserLoginCountAdapter.cpp',
      '<(main_project_path)/OceCxxAdapter/src/UserNameAdapter.cpp',
      '<(main_project_path)/OceCxxAdapter/src/UserPassportAdapter.cpp',
      '<(main_project_path)/OceCxxAdapter/src/UserRightAdapter.cpp',
      '<(main_project_path)/OceCxxAdapter/src/UserStageAdapter.cpp',
      '<(main_project_path)/OceCxxAdapter/src/UserStateAdapter.cpp',
      '<(main_project_path)/OceCxxAdapter/src/UserUrlAdapter.cpp',
      '<(main_project_path)/OceCxxAdapter/src/UserBornTripodAdapter.cpp',
      '<(main_project_path)/OceCxxAdapter/src/UserDoingTripodAdapter.cpp',
      '<(main_project_path)/OceCxxAdapter/src/UserLoginCountTripodAdapter.cpp',
      '<(main_project_path)/OceCxxAdapter/src/UserNameTripodAdapter.cpp',
      '<(main_project_path)/OceCxxAdapter/src/UserPassportTripodAdapter.cpp',
      '<(main_project_path)/OceCxxAdapter/src/UserRightTripodAdapter.cpp',
      '<(main_project_path)/OceCxxAdapter/src/UserStageTripodAdapter.cpp',
      '<(main_project_path)/OceCxxAdapter/src/UserStateTripodAdapter.cpp',
      '<(main_project_path)/OceCxxAdapter/src/UserUrlTripodAdapter.cpp',

    ],  
  },
  {
    'target_name' : 'tripodproducermanager',
    'type' : 'shared_library',
    'defines' : [],
    'dependencies' : [
      'tripodproducer',
      'xce_common.gyp:xce_common',
    ],
    'sources' : [
      '../tripod2/cc/producermanager/producer_manager_impl.cc',
    ],
  },
  {
    'target_name' : 'tripod_client',
    'type' : 'static_library',
    'defines' : [],
    'dependencies' : [
      'tripod_core',
      '3rdparty.gyp:hiredis',
      'base.gyp:base',
      'tripod_slice_and_proto',
    ],
    'sources' : [
      '../tripod2/cc/client/tripod_cache_client.cc',
      '../tripod2/cc/client/tripod_cache_client_factory.cc',
    ],
  },
  {
    'target_name' : 'tripod_client_test',
    'type' : 'executable',
    'defines' : [],
    'dependencies' : [
      'tripod_client',
      'xce_base.gyp:xce_base',
      'gtest.gyp:gtest',
      'gtest.gyp:gtest_main',
    ],
    'sources' : [
      '../tripod2/cc/client/test/tripod_cache_client_test.cc',
    ],
  }, # xce_cxx_adapter_test
  {
    'target_name' : 'xce_cxx_adapter_test',
    'type' : 'executable',
    'defines' : [],
    'dependencies' : [
      'tripod_core',
      'xce_base.gyp:xce_base',
      'gtest.gyp:gtest',
      'gtest.gyp:gtest_main',
    ],
    'sources' : [
      '<(main_project_path)/OceCxxAdapter/src/SocialGraphRcdTripodAdapter.cpp',
    ],
  }, # xce_cxx_adapter_test
  ]
}