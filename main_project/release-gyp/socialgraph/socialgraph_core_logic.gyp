{
	'includes' : [
		'socialgraph_common.gyp',
	],
  'variables': {
   'PROTOC_PATH':'/usr/local/distcache-dev/bin/protoc',
   'MAKE_TPL_VARNAMES_PATH':'/usr/local/xce-dev/bin/make_tpl_varnames_h',
   'ICE_PATH':'<!(echo $ICE_HOME)',
   'SLICE2CPP_PATH':'<(ICE_PATH)/bin/slice2cpp',
   'BOOST_PATH':'/usr/local/xce',
  },
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
				'-ljson_linux-gcc-4.1.2_libmt',
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
			'target_name' : 'SocialGraphCoreLogic',
			'type' : 'shared_library',
			'sources' : [
				'<(main_project_path)/socialgraph/socialgraph_core_logic/src/socialgraph_core_logicI.h',
				'<(main_project_path)/socialgraph/socialgraph_core_logic/src/socialgraph_core_logicI.cc',
				'<(main_project_path)/socialgraph/socialgraph_core_logic/src/sgmessage_handler.h',
				'<(main_project_path)/socialgraph/socialgraph_core_logic/src/sgmessage_handler.cc',
				'<(main_project_path)/socialgraph/socialgraphutil/config_parser2.h',
				'<(main_project_path)/socialgraph/socialgraphutil/config_parser2.cc',

				#### Cpp Files Generated from ICE ######
				'<(oce_slice_path)/RecommendationManager.cpp',
				'<(oce_slice_path)/DistributionManager.cpp',
				'<(oce_slice_path)/SocialGraphCoreLogic.cpp',
				'<(oce_slice_path)/Monitor.cpp',
				'<(oce_slice_path)/UserStateFilter.cpp',
				'<(oce_slice_path)/UserBase.cpp',
				'<(oce_slice_path)/GuideUserHighQualityUGC.cpp',

        '<(adapter_path)/SocialGraphRcdTripodAdapter.cpp',
        '<(adapter_path)/SocialGraphRcdPageCfTripodAdapter.cpp',
				'<(adapter_path)/DistributionManagerAdapter.cpp',
				'<(adapter_path)/UserStateAdapter.cpp',
				'<(adapter_path)/SocialGraphGuideUGCAdapter.cpp',
			],
			'dependencies' : [
			],
		},
	]
}
