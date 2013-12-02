{
        'includes' : [
		'socialgraph_common.gyp',
       	],
	'target_defaults' : {
  	        'target_defaults': {
                        'cflags':['-fPIC', '-O2'],
		},
		'include_dirs' : [
			'<(main_project_path)/third-party/include/',
			'<(main_project_path)/third-party/redis-c-driver',
		],
		'link_settings' : {
			'libraries' : [
				'-L /opt/jsoncpp-src-0.5.0/libs/linux-gcc-4.1.2',
			],
			'ldflags' : [
			],
		},
		'dependencies' : [
                        '../tripod2.gyp:tripod_core',
		],
	},
	'targets' : [
		{
			'target_name' : 'SocialFeedProducer',
			'type' : 'executable',
			'sources' : [
			'<(main_project_path)/socialgraph/SocialFeed/socialfeedupdator/src/main.cpp',
                        '<(main_project_path)/socialgraph/SocialFeed/socialfeedupdator/src/MyTasks.h',
                        '<(main_project_path)/socialgraph/SocialFeed/socialfeedupdator/src/MyTasks.cpp',
                        '<(main_project_path)/socialgraph/SocialFeed/socialfeedupdator/src/SocialFeed.h',
                        '<(main_project_path)/socialgraph/SocialFeed/socialfeedupdator/src/SocialFeed.cpp',
                        '<(main_project_path)/socialgraph/SocialFeed/socialfeedupdator/src/FriendFetcher.h',
                        '<(main_project_path)/socialgraph/SocialFeed/socialfeedupdator/src/FriendFetcher.cpp',
                        '<(main_project_path)/socialgraph/SocialFeed/socialfeedupdator/src/Common.h',	
                        '<(main_project_path)/socialgraph/SocialFeed/socialfeedupdator/src/MD5Util.h',	
                        '<(main_project_path)/socialgraph/SocialFeed/socialfeedupdator/src/MD5Util.cpp',	
                        
                          	#### Cpp Files Generated from ICE ######
                                
                                #### Cpp Files Generated from ICE ######
                                '<(adapter_path)/SocialGraphCommonTripodAdapter.h',
                                '<(adapter_path)/SocialGraphCommonTripodAdapter.cpp',
			],
			'dependencies' : [
			],
		},
	]
}
