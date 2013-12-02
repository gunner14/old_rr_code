{
	'includes' : [
		'socialgraph_common.gyp',
	],
	'target_defaults' : {
		'include_dirs' : [
			'<(main_project_path)/third-party/gtest/include',					#special path for gtest.gyp
			'<(main_project_path)/third-party/include/',
	    '<(main_project_path)/third-party/redis-c-driver',
		],
		'link_settings' : {
			'libraries' : [
				'-L ../third-party/lib',
				'-ljson_linux-gcc-4.1.2_libmt',
			],
			'ldflags' : [
			],
		},
		'dependencies' : [
  		'../gtest.gyp:gtest',					#add gtest dependencies
  		'../gtest.gyp:gtest_main'
		],
	},
	'targets' : [
		{
			'target_name' : 'ExperimentITest',
			'type' : 'executable',
			'sources' : [
				'<(main_project_path)/socialgraph/recommendation_online/test/main.cc',
				'<(main_project_path)/socialgraph/recommendation_online/builder/base_builder.h',
				'<(main_project_path)/socialgraph/recommendation_online/builder/base_builder.cc',
				'<(main_project_path)/socialgraph/recommendation_online/builder/home_impl/friend_recommend_builder.h',
				'<(main_project_path)/socialgraph/recommendation_online/builder/home_impl/friend_recommend_builder.cc',
				'<(main_project_path)/socialgraph/recommendation_online/builder/home_impl/commondbhelper.h',
				'<(main_project_path)/socialgraph/recommendation_online/builder/home_impl/commondbhelper.cc',
				
				#### Cpp Files Generated from ICE ######
				'<(oce_slice_path)/DistributionManager.cpp',
				'<(oce_slice_path)/RecommendationManager.cpp',
				'<(oce_slice_path)/Monitor.cpp',

				#### Cpp Files Generated from ICE ######
				'<(adapter_path)/DistributionManagerAdapter.cpp',
				'<(adapter_path)/RecommendationOnlineAdapter.cpp',
				'<(adapter_path)/RecommendationFeedOnlineAdapter.cpp',
				'<(adapter_path)/RecommendationFeedExperimentIAdapter.cpp',
				'<(adapter_path)/RecommendationManangerAdapter.cpp',
				'<(adapter_path)/RecommendationExperimentIAdapter.cpp',
				'<(adapter_path)/RecommendationExperimentIIAdapter.cpp',
				'<(adapter_path)/RecommendationExperimentIIIAdapter.cpp',
				'<(adapter_path)/RecommendationExperimentIVAdapter.cpp',
				'<(adapter_path)/RecommendationExperimentVAdapter.cpp',
				'<(adapter_path)/RecommendationExperimentVIIIIAdapter.cpp',
				'<(adapter_path)/RecommendationExperimentVVAdapter.cpp',
				'<(adapter_path)/MonitorAdapter.cpp',
				'<(adapter_path)/SocialGraphRcdTripodAdapter.cpp',
			],
			'dependencies' : [
				'../3rdparty.gyp:hiredis',
      	'../base.gyp:base',
      	'../xce_base.gyp:xce_base',
      	'../tripod2.gyp:tripod_core',
			],
		},
		{
			'target_name' : 'ConfigParserTest',
			'type' : 'executable',
			'sources' : [
				'<(main_project_path)/socialgraph/recommendation_online/test/config_parser_test.cc',
				'<(main_project_path)/socialgraph/recommendation_online/common/config_parser2.h',
				'<(main_project_path)/socialgraph/recommendation_online/common/config_parser2.cc',
				
				#### Cpp Files Generated from ICE ######
				'<(oce_slice_path)/Monitor.cpp',

				#### Cpp Files Generated from ICE ######
				'<(adapter_path)/MonitorAdapter.cpp',
			],
		},
		{
			'target_name' : 'IdFilterTest',
			'type' : 'executable',
			'sources' : [
				'<(main_project_path)/socialgraph/recommendation_online/test/id_filter_test.cc',
				'<(main_project_path)/socialgraph/recommendation_online/common/id_filter.h',
				'<(main_project_path)/socialgraph/recommendation_online/common/id_filter.cc',
				
				#### Cpp Files Generated from ICE ######
				'<(oce_slice_path)/Monitor.cpp',

				#### Cpp Files Generated from ICE ######
				'<(adapter_path)/MonitorAdapter.cpp',
			],
		},
		{
			'target_name' : 'BreakpadTest',
			'type' : 'executable',
			'cflags':['-g'],
			'include_dirs' : [
				'<(main_project_path)/third-party/google-breakpad/src',					#special path for gtest.gyp
			],
			'link_settings' : {
				'libraries' : [
					'-L /usr/local/xce-dev/lib',
					'-lbreakpad',
				],
				'ldflags' : [
				],
			},
			'sources' : [
				'<(main_project_path)/socialgraph/socialgraphutil/test/breakpad_test.cc',
				
				#### Cpp Files Generated from ICE ######
				'<(oce_slice_path)/Monitor.cpp',

				#### Cpp Files Generated from ICE ######
				'<(adapter_path)/MonitorAdapter.cpp',
			],
		},
		{
			'target_name' : 'SocialGraphCoreLogicTest',
			'type' : 'executable',
			'cflags':['-g'],
			'link_settings' : {
				'libraries' : [
					'-L /usr/local/xce-dev/lib',
				],
				'ldflags' : [
				],
			},
			'sources' : [
				'<(main_project_path)/socialgraph/socialgraph_core_logic/test/socialgraphcorelogic_test.cc',
				
				#### Cpp Files Generated from ICE ######
				'<(oce_slice_path)/Monitor.cpp',
				'<(oce_slice_path)/SocialGraphCoreLogic.cpp',

				#### Cpp Files Generated from ICE ######
				'<(adapter_path)/MonitorAdapter.cpp',
				'<(adapter_path)/SocialGraphCoreLogicAdapter.cpp',
			],
		},
                {
                        'target_name' : 'PageCacheWorker',
                        'type' : 'executable',
                        'sources' : [
                                '<(main_project_path)/socialgraph/recommendcontentupdator/PageModelCreator/RcdPageWriter/main.cc',
                                '<(main_project_path)/socialgraph/recommendcontentupdator/PageModelCreator/RcdPageCacheHelper/pagecachehelper.cc',
                                '<(main_project_path)/socialgraph/recommendcontentupdator/PageModelCreator/RcdPageCacheHelper/pagecachehelper.h',
                                '<(main_project_path)/socialgraph/recommendcontentupdator/PageModelCreator/RcdPageDbHelper/rcdpagedbhelper.cc',
                                '<(main_project_path)/socialgraph/recommendcontentupdator/PageModelCreator/RcdPageDbHelper/rcdpagedbhelper.h',
                                '<(main_project_path)/socialgraph/recommendcontentupdator/PageModelCreator/RcdPageWriter/PageCacheTask.cc',
                                '<(main_project_path)/socialgraph/recommendcontentupdator/PageModelCreator/RcdPageWriter/PageCacheTask.h',
                                #### Cpp Files Generated from ICE ######
                                '<(oce_slice_path)/Monitor.cpp',
				'<(oce_slice_path)/BuddyByIdCache.cpp', 
                                #### Cpp Files Generated from ICE ######
				'<(adapter_path)/BuddyByIdReplicaAdapter.cpp',
				'<(adapter_path)/SocialGraphRcdPageCfTripodAdapter.cpp',
 

                        ],
                        'dependencies' : [
                                '../3rdparty.gyp:hiredis',
        '../base.gyp:base',
        '../xce_base.gyp:xce_base',
        '../tripod2.gyp:tripod_core',
                        ],
                },
		{
			'target_name' : 'FeedExperimentITest',
			'type' : 'executable',
			'sources' : [
				'<(main_project_path)/socialgraph/recommendation_online/test/feed_main.cc',
				'<(main_project_path)/socialgraph/recommendation_online/builder/base_builder.h',
				'<(main_project_path)/socialgraph/recommendation_online/builder/base_builder.cc',
				'<(main_project_path)/socialgraph/recommendation_online/builder/feed_impl/feed_recommend_builder.h',
				'<(main_project_path)/socialgraph/recommendation_online/builder/feed_impl/feed_recommend_builder.cc',
				
				#### Cpp Files Generated from ICE ######
				'<(oce_slice_path)/DistributionManager.cpp',
				'<(oce_slice_path)/RecommendationManager.cpp',
				'<(oce_slice_path)/Monitor.cpp',

				#### Cpp Files Generated from ICE ######
				'<(adapter_path)/DistributionManagerAdapter.cpp',
				'<(adapter_path)/RecommendationOnlineAdapter.cpp',
				'<(adapter_path)/RecommendationFeedOnlineAdapter.cpp',
				'<(adapter_path)/RecommendationFeedExperimentIAdapter.cpp',
				'<(adapter_path)/RecommendationManangerAdapter.cpp',
				'<(adapter_path)/RecommendationFeedExperimentIAdapter.cpp',
				'<(adapter_path)/RecommendationExperimentIAdapter.cpp',
				'<(adapter_path)/RecommendationExperimentIIAdapter.cpp',
				'<(adapter_path)/RecommendationExperimentIIIAdapter.cpp',
				'<(adapter_path)/RecommendationExperimentIVAdapter.cpp',
				'<(adapter_path)/RecommendationExperimentVAdapter.cpp',
				'<(adapter_path)/MonitorAdapter.cpp',
				'<(adapter_path)/SocialGraphRcdTripodAdapter.cpp',
				'<(adapter_path)/SocialGraphRcdPageCfTripodAdapter.cpp',
			],
			'dependencies' : [
				'../3rdparty.gyp:hiredis',
      	'../base.gyp:base',
      	'../xce_base.gyp:xce_base',
      	'../tripod2.gyp:tripod_core',
			],
		},
		
		{
			'target_name' : 'UserStateTest',
			'type' : 'executable',
			'sources' : [
				'<(main_project_path)/socialgraph/recommendation_online/test/user_state.cc',
				
				#### Cpp Files Generated from ICE ######
				'<(oce_slice_path)/UserStateFilter.cpp',
				'<(oce_slice_path)/UserBase.cpp',
				'<(oce_slice_path)/DistributionManager.cpp',
				'<(oce_slice_path)/RecommendationManager.cpp',
				'<(oce_slice_path)/Monitor.cpp',

				#### Cpp Files Generated from ICE ######
				'<(adapter_path)/DistributionManagerAdapter.cpp',
				'<(adapter_path)/RecommendationOnlineAdapter.cpp',
				'<(adapter_path)/RecommendationFeedOnlineAdapter.cpp',
				'<(adapter_path)/RecommendationFeedExperimentIAdapter.cpp',
				'<(adapter_path)/RecommendationManangerAdapter.cpp',
				'<(adapter_path)/RecommendationFeedExperimentIAdapter.cpp',
				'<(adapter_path)/RecommendationExperimentIAdapter.cpp',
				'<(adapter_path)/RecommendationExperimentIIAdapter.cpp',
				'<(adapter_path)/RecommendationExperimentIIIAdapter.cpp',
				'<(adapter_path)/RecommendationExperimentIVAdapter.cpp',
				'<(adapter_path)/RecommendationExperimentVAdapter.cpp',
				'<(adapter_path)/MonitorAdapter.cpp',
				'<(adapter_path)/SocialGraphRcdTripodAdapter.cpp',
				'<(adapter_path)/SocialGraphRcdPageCfTripodAdapter.cpp',
				'<(adapter_path)/UserStateAdapter.cpp',
			],
			'dependencies' : [
				'../3rdparty.gyp:hiredis',
      	'../base.gyp:base',
      	'../xce_base.gyp:xce_base',
      	'../tripod2.gyp:tripod_core',
			],
		},
	]
}
