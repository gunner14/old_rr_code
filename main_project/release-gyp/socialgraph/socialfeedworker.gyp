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
                        './socialgraph_util.gyp:socialgraph_util',
		],
	},
	'targets' : [
		{
			'target_name' : 'SocialFeedWorker',
			'type' : 'shared_library',
			'sources' : [
		  	        '<(main_project_path)/socialgraph/SocialFeed/socialfeedworker/worker/socialfeedworker.cc',
                                '<(main_project_path)/socialgraph/SocialFeed/socialfeedworker/common/config_parser.cc',
				'<(main_project_path)/socialgraph/SocialFeed/socialfeedworker/builder/commondbhelper.cc',
				'<(main_project_path)/socialgraph/SocialFeed/socialfeedworker/builder/feed_recommend_builder.cc',
				'<(main_project_path)/socialgraph/SocialFeed/socialfeedworker/builder/base_builder.cc',
				'<(main_project_path)/socialgraph/SocialFeed/socialfeedworker/builder/feed_push_task.cc',
				'<(main_project_path)/socialgraph/socialgraphd/headurl.cc',

				#### Cpp Files Generated from ICE ######
                                '<(oce_slice_path)/FeedCreator.h',
                                '<(oce_slice_path)/FeedCreator.cpp',
                                '<(oce_slice_path)/RFeed.h',
                                '<(oce_slice_path)/RFeed.cpp',
				'<(oce_slice_path)/SocialFeedRecommendationManager.h',
				'<(oce_slice_path)/SocialFeedRecommendationManager.cpp',
                                ### Cpp for Adapter
                                '<(adapter_path)/SocialGraphCommonTripodAdapter.cpp',
                                '<(adapter_path)/FeedCreatorAdapter.h',
                                '<(adapter_path)/FeedCreatorAdapter.cpp',
                                '<(adapter_path)/UserNameTripodAdapter.h',
                                '<(adapter_path)/UserWorkplaceTripodAdapter.cpp',
                                '<(adapter_path)/UserUniversityTripodAdapter.cpp',
			],
			'dependencies' : [ 
                                '../user/user_slice_and_adapter.gyp:*',
			 ],
		},
	]
}
