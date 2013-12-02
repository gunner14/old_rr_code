{
	'includes' : [
		'socialgraph_common.gyp',
                '../user/user_common.gyp',
	],
        'target_defaults' : {
		'target_defaults': {
                        'cflags':['-fPIC', '-O2'],
	  	},
		'include_dirs' : [
			'<(main_project_path)/third-party/include/',
			'<(main_project_path)/third-party/redis-c-driver',
                        '<(main_project_path)/third-party/libactivemq/include/activemq-cpp-3.4.1',
		],
		'link_settings' : {
			'libraries' : [
				'-L /opt/jsoncpp-src-0.5.0/libs/linux-gcc-4.1.2',
                                '-ljson_linux-gcc-4.1.2_libmt',
                                '-L <(main_project_path)/third-party/libactivemq/lib',
                                '-lactivemq-cpp',
			],
			'ldflags' : [
			],
		},
		'dependencies' : [
			'../3rdparty.gyp:hiredis',
			'../base.gyp:base',
			'../xce_base.gyp:xce_base',
			'../tripod2.gyp:tripod_core',
		],
	},
	'targets' : [
		{
			'target_name' : 'NewRecommendPage',
			'type' : 'executable',
			'sources' : [
				'<(main_project_path)/socialgraph/newrecommendpage/src/Common.h',
				'<(main_project_path)/socialgraph/newrecommendpage/src/FriendFetcher.h',
				'<(main_project_path)/socialgraph/newrecommendpage/src/FriendFetcher.cpp',
				'<(main_project_path)/socialgraph/newrecommendpage/src/main.cc',
				#### Cpp Files Generated from ICE ######
                                '<(oce_slice_path)/UserNetwork.h',
                                '<(oce_slice_path)/UserNetwork.cpp',
                                ### Cpp for Adapter
                                '<(adapter_path)/UserUniversityTripodAdapter.cpp',
			],
			'dependencies' : [ 
                                '../user/user_slice_and_adapter.gyp:*',
			 ],
		},
	]
}
