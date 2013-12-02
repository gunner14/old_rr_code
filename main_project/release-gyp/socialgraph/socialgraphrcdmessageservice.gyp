{
	'includes' : [
		'socialgraph_common.gyp',
	],
	'targets' : [
	    {
			'target_name' : 'SocialGraphRcdMessageService',
			'type' : 'shared_library',
			'include_dirs': [
				'<(main_project_path)/third-party/include',
				'<(main_project_path)/third-party/redis-c-driver',
			],
			'sources' : [
				'<(main_project_path)/socialgraph/sg_rcd/message_service/rcd_message_service.h',
				'<(main_project_path)/socialgraph/sg_rcd/message_service/rcd_message_service.cc',

				#### Cpp Files Generated from ICE ######
				'<(oce_slice_path)/SocialGraphRcdMessageService.cpp',
				'<(oce_slice_path)/Monitor.cpp',
        	    '<(adapter_path)/SocialGraphRcdTripodAdapter.cpp',
        	    '<(adapter_path)/SocialGraphRcdMessageServiceAdapter.cpp',
			],
			'dependencies' : [
			  '../3rdparty.gyp:hiredis',
			  '../base.gyp:base',
			  '../xce_base.gyp:xce_base',
			  '../tripod2.gyp:tripod_core',
			],
		},

		{
			'target_name' : 'SocialGraphRcdMessageService_Test',
			'type' : 'executable',
			'include_dirs': [
				'../third-party/redis-c/driver'
			],
			'sources' : [
				'<(main_project_path)/socialgraph/sg_rcd/test/test.cc',
				'<(main_project_path)/socialgraph/sg_rcd/message_service/rcd_message_service.h',
				'<(main_project_path)/socialgraph/sg_rcd/message_service/rcd_message_service.cc',

				#### Cpp Files Generated from ICE ######
				'<(oce_slice_path)/SocialGraphRcdMessageService.cpp',
				'<(oce_slice_path)/Monitor.cpp',
        	    '<(adapter_path)/SocialGraphRcdTripodAdapter.cpp',
        	    '<(adapter_path)/SocialGraphRcdMessageServiceAdapter.cpp',
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
