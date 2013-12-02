
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
			'target_name' : 'SortByCluster',
			'type' : 'shared_library',
	#		'type' : 'executable',
			'sources' : [
				'<(main_project_path)/socialgraph/sortbycluster/server/autogroup.h',
				'<(main_project_path)/socialgraph/sortbycluster/server/autogroup.cc',
				'<(main_project_path)/socialgraph/sortbycluster/server/autogroupwithfree.h',
				'<(main_project_path)/socialgraph/sortbycluster/server/autogroupwithfree.cc',
				'<(main_project_path)/socialgraph/sortbycluster/server/friendinfoutil.h',
				'<(main_project_path)/socialgraph/sortbycluster/server/friendinfoutil.cpp',
				'<(main_project_path)/socialgraph/sortbycluster/server/friendsgraph.h',
				'<(main_project_path)/socialgraph/sortbycluster/server/group.h',
				'<(main_project_path)/socialgraph/sortbycluster/server/genericsortutil.h',
				'<(main_project_path)/socialgraph/sortbycluster/server/genericsortutil.cc',
				'<(main_project_path)/socialgraph/sortbycluster/server/sort_util.h',
				'<(main_project_path)/socialgraph/sortbycluster/server/sort_util.cc',
				'<(main_project_path)/socialgraph/sortbycluster/server/sortbyclusterI.h',
				'<(main_project_path)/socialgraph/sortbycluster/server/sortbyclusterI.cc',
				
				#### Cpp Files Generated from ICE ######
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
        '<(oce_slice_path)/OfferFriends.cpp',
        '<(oce_slice_path)/SortByCluster.cpp',
        '<(oce_slice_path)/UserBase.cpp',

        #### Cpp Files Generated from ICE ######
        '<(adapter_path)/SocialGraphRcdTripodAdapter.cpp',
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
        '<(adapter_path)/OfferFriendsAdapter.cpp',
        '<(adapter_path)/UserBaseAdapter.cpp',
        '<(adapter_path)/UserBaseReaderAdapter.cpp',
        '<(adapter_path)/UserBaseTripodAdapter.cpp',
        '<(adapter_path)/UserNameAdapter.cpp',
			],
			'dependencies' : [
			],
		},

    {
			'target_name' : 'SortByClusterTest',
			'type' : 'executable',
			'sources' : [
				'<(main_project_path)/socialgraph/sortbycluster/server/autogroup.h',
				'<(main_project_path)/socialgraph/sortbycluster/server/autogroup.cc',
				'<(main_project_path)/socialgraph/sortbycluster/server/autogroupwithfree.h',
				'<(main_project_path)/socialgraph/sortbycluster/server/autogroupwithfree.cc',
				'<(main_project_path)/socialgraph/sortbycluster/server/friendinfoutil.h',
				'<(main_project_path)/socialgraph/sortbycluster/server/friendinfoutil.cpp',
				'<(main_project_path)/socialgraph/sortbycluster/server/friendsgraph.h',
				'<(main_project_path)/socialgraph/sortbycluster/server/group.h',
				'<(main_project_path)/socialgraph/sortbycluster/server/genericsortutil.h',
				'<(main_project_path)/socialgraph/sortbycluster/server/genericsortutil.cc',
				'<(main_project_path)/socialgraph/sortbycluster/server/sort_util.h',
				'<(main_project_path)/socialgraph/sortbycluster/server/sort_util.cc',
				'<(main_project_path)/socialgraph/sortbycluster/server/sortbyclusterI.h',
				'<(main_project_path)/socialgraph/sortbycluster/server/sortbyclusterI.cc',
				'<(main_project_path)/socialgraph/sortbycluster/server/main.cpp',
				
				#### Cpp Files Generated from ICE ######
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
        '<(oce_slice_path)/OfferFriends.cpp',
        '<(oce_slice_path)/SortByCluster.cpp',
        '<(oce_slice_path)/UserBase.cpp',


        #### Cpp Files Generated from ICE ######
        '<(adapter_path)/SocialGraphRcdTripodAdapter.cpp',
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
        '<(adapter_path)/OfferFriendsAdapter.cpp',
        '<(adapter_path)/UserBaseAdapter.cpp',
        '<(adapter_path)/UserNameAdapter.cpp',
        '<(adapter_path)/UserBaseReaderAdapter.cpp',
        '<(adapter_path)/UserBaseTripodAdapter.cpp',

			],
			'dependencies' : [
			],
		},
  ]
}
