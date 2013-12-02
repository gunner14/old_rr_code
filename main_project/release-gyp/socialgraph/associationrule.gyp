
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
			#	'-ljson_linux-gcc-4.1.2_libmt',
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
			'target_name' : 'AssociationRuleTest',
	#		'type' : 'shared_library',
			'type' : 'executable',
			'sources' : [
				'<(main_project_path)/socialgraph/AssociationRule/recommend/AssociationRuleI.h',
				'<(main_project_path)/socialgraph/AssociationRule/recommend/AssociationRuleI.cc',
				'<(main_project_path)/socialgraph/AssociationRule/share/AssociationRuleUtil.h',
				'<(main_project_path)/socialgraph/AssociationRule/share/AssociationRuleUtil.cc',
				'<(main_project_path)/socialgraph/AssociationRule/share/ARCacheUtil.h',
				'<(main_project_path)/socialgraph/AssociationRule/share/ARCacheUtil.cc',
				'<(main_project_path)/socialgraph/AssociationRule/share/ARCreator.h',
				'<(main_project_path)/socialgraph/AssociationRule/share/ARCreator.cc',
				'<(main_project_path)/socialgraph/AssociationRule/test/main.cc',
				
				#### Cpp Files Generated from ICE ######
        '<(oce_slice_path)/AssociationRule.cpp',
        '<(oce_slice_path)/FriendFinderInfoCache.cpp',
        '<(oce_slice_path)/BuddyByIdCache.cpp',
        '<(oce_slice_path)/FriendCountCache.cpp',
        '<(oce_slice_path)/FriendFinder.cpp',
        '<(oce_slice_path)/FriendFinderInfoCache.cpp',
        '<(oce_slice_path)/FriendFinderByIP.cpp',
        '<(oce_slice_path)/IPRecord.cpp',
        '<(oce_slice_path)/PhotoStateFilter.cpp',
        '<(oce_slice_path)/OnlineStateFilter.cpp',
        '<(oce_slice_path)/StatusCache.cpp',
        '<(oce_slice_path)/UserStateFilter.cpp',
        '<(oce_slice_path)/FriendRankNew.cpp',
        '<(oce_slice_path)/BuddyByApplicantCache.cpp',

        #### Cpp Files Generated from ICE ######
        '<(adapter_path)/BuddyByIdReplicaAdapter.cpp',
        '<(adapter_path)/FriendCountCacheReplicaAdapter.cpp',
        '<(adapter_path)/FriendFinderReplicaAdapter.cpp',
        '<(adapter_path)/FriendFinderInfoCacheReplicaAdapter.cpp',
        '<(adapter_path)/FriendFinderByIPReplicaAdapter.cpp',
        '<(adapter_path)/IPRecordReplicaAdapter.cpp',
        '<(adapter_path)/PhotoStateFilterAdapter.cpp',
        '<(adapter_path)/OnlineStateFilterAdapter.cpp',
        '<(adapter_path)/StatusCacheAdapter.cpp',
        '<(adapter_path)/UserStateFilterAdapter.cpp',
        '<(adapter_path)/SocialGraphCommonTripodAdapter.cpp',
        '<(adapter_path)/FriendRankCacheNewAdapter.cpp',
        '<(adapter_path)/BuddyByApplicantAdapter.cpp',
        '<(adapter_path)/AssociationRuleAdapter.cpp',
			],
			'dependencies' : [
			],
		},

		{
			'target_name' : 'AssociationRule',
			'type' : 'shared_library',
	#		'type' : 'executable',
			'sources' : [
				'<(main_project_path)/socialgraph/AssociationRule/recommend/AssociationRuleI.h',
				'<(main_project_path)/socialgraph/AssociationRule/recommend/AssociationRuleI.cc',
				'<(main_project_path)/socialgraph/AssociationRule/share/AssociationRuleUtil.h',
				'<(main_project_path)/socialgraph/AssociationRule/share/AssociationRuleUtil.cc',
				'<(main_project_path)/socialgraph/AssociationRule/share/ARCacheUtil.h',
				'<(main_project_path)/socialgraph/AssociationRule/share/ARCacheUtil.cc',
				'<(main_project_path)/socialgraph/AssociationRule/share/ARCreator.h',
				'<(main_project_path)/socialgraph/AssociationRule/share/ARCreator.cc',
				
				#### Cpp Files Generated from ICE ######
        '<(oce_slice_path)/AssociationRule.cpp',
        '<(oce_slice_path)/FriendFinderInfoCache.cpp',
        '<(oce_slice_path)/BuddyByIdCache.cpp',
        '<(oce_slice_path)/FriendCountCache.cpp',
        '<(oce_slice_path)/FriendFinder.cpp',
        '<(oce_slice_path)/FriendFinderInfoCache.cpp',
        '<(oce_slice_path)/FriendFinderByIP.cpp',
        '<(oce_slice_path)/IPRecord.cpp',
        '<(oce_slice_path)/PhotoStateFilter.cpp',
        '<(oce_slice_path)/OnlineStateFilter.cpp',
        '<(oce_slice_path)/StatusCache.cpp',
        '<(oce_slice_path)/UserStateFilter.cpp',
        '<(oce_slice_path)/FriendRankNew.cpp',
        '<(oce_slice_path)/BuddyByApplicantCache.cpp',

        #### Cpp Files Generated from ICE ######
        '<(adapter_path)/BuddyByIdReplicaAdapter.cpp',
        '<(adapter_path)/FriendCountCacheReplicaAdapter.cpp',
        '<(adapter_path)/FriendFinderReplicaAdapter.cpp',
        '<(adapter_path)/FriendFinderInfoCacheReplicaAdapter.cpp',
        '<(adapter_path)/FriendFinderByIPReplicaAdapter.cpp',
        '<(adapter_path)/IPRecordReplicaAdapter.cpp',
        '<(adapter_path)/PhotoStateFilterAdapter.cpp',
        '<(adapter_path)/OnlineStateFilterAdapter.cpp',
        '<(adapter_path)/StatusCacheAdapter.cpp',
        '<(adapter_path)/UserStateFilterAdapter.cpp',
        '<(adapter_path)/SocialGraphCommonTripodAdapter.cpp',
        '<(adapter_path)/FriendRankCacheNewAdapter.cpp',
        '<(adapter_path)/BuddyByApplicantAdapter.cpp',
			],
			'dependencies' : [
			],
		},
  ]
}
