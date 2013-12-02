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
				'-L /data/xce/XSocialGraph/shared_lib64',
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
                        './socialgraph_util.gyp:socialgraph_util',
		],
	},
	'targets' : [
		{
			'target_name' : 'GuideUGCReshow',
			'type' : 'shared_library',
			'sources' : [
		  	        '<(main_project_path)/socialgraph/GuideHighQualityUGC/reshow/guideuserUGCreshow.cc',
		  	        '<(main_project_path)/socialgraph/GuideHighQualityUGC/reshow/test.cc',
                                '<(main_project_path)/socialgraph/GuideHighQualityUGC/common/config_parser.cc',
			        '<(main_project_path)/socialgraph/GuideHighQualityUGC/builder/Common.h',
			        '<(main_project_path)/socialgraph/GuideHighQualityUGC/builder/FriendFetcher.cpp',
			      	'<(main_project_path)/socialgraph/GuideHighQualityUGC/builder/ugc_feed_builder.cc',

				#### Cpp Files Generated from ICE ######
                                '<(oce_slice_path)/RFeed.h',
                                '<(oce_slice_path)/RFeed.cpp',
				'<(oce_slice_path)/TimeLineFeed.h',
				'<(oce_slice_path)/TimeLineFeed.cpp',
				'<(oce_slice_path)/GuideUserHighQualityUGC.h',
				'<(oce_slice_path)/GuideUserHighQualityUGC.cpp',
                                ### Cpp for Adapter
                                '<(adapter_path)/UserStateInfo.h',
                                '<(adapter_path)/TimeLineFeedReplicaAdapter.cpp',
                                '<(adapter_path)/FeedInterfaceReplicaAdapter.cpp',
                           #     '<(adapter_path)/UserStateTripodAdapter.cpp',
			],
			'dependencies' : [ 
                                '../user/user_slice_and_adapter.gyp:*',
			 ],
		},
	]
}
