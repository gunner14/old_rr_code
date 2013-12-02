{
  #包含公共设置
  'includes':[
		'socialgraph_common.gyp',
  ],
  'target_defaults' : {
    'include_dirs' : [
    #  '/usr/local/distcache-dev/include',
    #  '<(main_project_path)/tripod-new/src/cpp/include',
    #  '<(main_project_path)/TalkUtil/src',
      '<(main_project_path)/third-party/include/',
    ],
		'link_settings' : {
			'libraries' : [
			'-L /opt/jsoncpp-src-0.5.0/libs/linux-gcc-4.1.2',
			'-ljson_linux-gcc-4.1.2_libmt'
			],
			'ldflags' : [
			],
		},
  },

  'targets' : [
  	{
  	  'target_name' : 'ConfigParser',
  	  'type' : 'executable',
  	  'sources' : [
				'<(main_project_path)/socialgraph/friendrecommendmanager/configparser/main.cc',
				'<(main_project_path)/socialgraph/friendrecommendmanager/configparser/config_parser.h',
				'<(main_project_path)/socialgraph/friendrecommendmanager/configparser/config_parser.cc',
  	  ],
  	  'dependencies' : [
  	  ],
  	}, 
  ] #end targets
}

