{
	'includes' : [
		'../global_vars.gyp',
	],
  'variables': {
   'PROTOC_PATH':'/usr/local/distcache-dev/bin/protoc',
   'MAKE_TPL_VARNAMES_PATH':'/usr/local/xce-dev/bin/make_tpl_varnames_h',
   'ICE_PATH':'<!(echo $ICE_HOME)',
   'SLICE2CPP_PATH':'<(ICE_PATH)/bin/slice2cpp',
    'BOOST_PATH':'/usr/local/xce',
  },
 'conditions':[
    ['OS=="linux"', {
     'target_defaults': {
       'cflags':['-fPIC', '-g', '-O2'],
       'defines':['OS_LINUX', 'POSIX', 'NEWARCH'],
       'include_dirs':[
         '<(BOOST_PATH)/include/boost-1_38',
         '../..',
         '<(ICE_PATH)/include',
         '/usr/include/mysql',
         '/usr/local/xce/include',
         '/usr/local/distcache/include',
         '/usr/local/xce-dev/include',
         '/usr/local/distcache-dev/include',    # TODO: maybe wrong?
         '/usr/local/distcache-util/include',
         '/opt/mongo-cxx-driver/include',
         ],
        },
      },
    ],
    ['OS=="win"', {'target_defaults': {},},],
  ],
  'targets': [
   {
   'target_name': 'socialgraph_util',
   'type': '<(library)',
   'defines' : ['SGTEST', 'GYP_COMPILE'],
   'include_dirs':[
      '<(main_project_path)/OceProto/proto/',
      '<(main_project_path)/IceExt/src',
      '<(main_project_path)/UtilCxx/src',
      '<(main_project_path)/UtilSlice/slice',
      '<(main_project_path)/OceSlice/slice',
      '<(main_project_path)/DbCxxPool/src',
      '<(main_project_path)/third-party/redis-c-driver/'
   ],
   'direct_dependent_settings': {
     'target_conditions': [
       ['_type=="executable"', {
         'conditions':[
           ['OS=="linux"',{
             'libraries': [
               '-L/usr/local/distcache-dev/lib/', '-lzookeeper_mt',
               '-L/usr/local/distcache-util/lib/', '-lrdc-client',
             ], 
             'defines' : ['GYP_COMPILE'],
           }],
         ],
       }],
     ],
   },
   'include_dirs' : [
     '<(main_project_path)/third-party/include/',
     '<(main_project_path)/third-party/redis-c-driver',
   ],
   'link_settings' : {
     'libraries' : [
       '-L /opt/jsoncpp-src-0.5.0/libs/linux-gcc-4.1.2',
       '-ljson_linux-gcc-4.1.2_libmt',
     ],
     'ldflags' : [
     ],
   },
   'sources': [
			'<(main_project_path)/socialgraph/socialgraphutil/redis_adapter.h',
			'<(main_project_path)/socialgraph/socialgraphutil/redis_adapter.cc',
			'<(main_project_path)/socialgraph/socialgraphutil/redisclient.h',
			'<(main_project_path)/socialgraph/socialgraphutil/redisclient.cc',
			'<(main_project_path)/socialgraph/socialgraphutil/mcclient.h',
			'<(main_project_path)/socialgraph/socialgraphutil/mcclient.cc',
			'<(main_project_path)/socialgraph/socialgraphutil/algostuff.h',
			'<(main_project_path)/socialgraph/socialgraphutil/algostuff.cc',
			'<(main_project_path)/socialgraph/socialgraphutil/mc_adapter.h',
			'<(main_project_path)/socialgraph/socialgraphutil/mc_adapter.cc',
			'<(main_project_path)/socialgraph/socialgraphutil/dbutil.h',
			'<(main_project_path)/socialgraph/socialgraphutil/dbutil.cc',
			'<(main_project_path)/socialgraph/socialgraphutil/zkclient.h',
			'<(main_project_path)/socialgraph/socialgraphutil/zkclient.cc',
			'<(main_project_path)/socialgraph/socialgraphutil/constants.h',
			'<(main_project_path)/socialgraph/socialgraphutil/constants.cc',
			'<(main_project_path)/socialgraph/socialgraphproto/item.pb.h',
			'<(main_project_path)/socialgraph/socialgraphproto/friendcluster.pb.h',
			'<(main_project_path)/socialgraph/socialgraphproto/item.pb.cc',
			'<(main_project_path)/socialgraph/socialgraphproto/friendcluster.pb.cc',
			'<(main_project_path)/socialgraph/socialgraphproto/pukfriendrecommend.pb.h',
			'<(main_project_path)/socialgraph/socialgraphproto/pukfriendrecommend.pb.cc',
    ],
    'dependencies' : [
      '../oce_slice.gyp:oce_slice',
      '../oce_proto.gyp:oce_proto',
      '../xce_base.gyp:util_slice_ice',
      '../xce_base.gyp:util_slice',
      '../xce_base.gyp:xce_base',
      '../xce_base.gyp:db_cxx_pool',
      '../base.gyp:base',
			'socialgraph_proto.gyp:socialgraph_proto',
    ],
   },
  ]
}

