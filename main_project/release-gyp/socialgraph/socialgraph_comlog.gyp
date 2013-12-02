{
  #包含公共设置
  'includes':[
		'socialgraph_common.gyp',
  ],
  'target_defaults' : {
    'include_dirs' : [
      '<(main_project_path)/third-party/log4cplus/include/',
    ],
		'link_settings' : {
			'libraries' : [
			'-L <(main_project_path)/third-party/log4cplus/lib/',
			'-llog4cplus'
			],
			'ldflags' : [
			],
		},
  },

  'targets' : [
  	{
  	  'target_name' : 'comlog',
  	  'type' : 'static_library',
  	  'sources' : [
				'<(main_project_path)/socialgraph/comlog/comlogcommon.h',
				'<(main_project_path)/socialgraph/comlog/comlogmain.h',
				'<(main_project_path)/socialgraph/comlog/comlogmain.cpp',
				'<(main_project_path)/socialgraph/comlog/comlogaddpender.h',
				'<(main_project_path)/socialgraph/comlog/comlogaddpender.cpp',
				'<(main_project_path)/socialgraph/comlog/comlogfilter.h',
				'<(main_project_path)/socialgraph/comlog/comlogfilter.cpp',
				'<(main_project_path)/socialgraph/comlog/comloglayout.h',
				'<(main_project_path)/socialgraph/comlog/comloglayout.cpp',
				'<(main_project_path)/socialgraph/comlog/comlogconfig.h',
				'<(main_project_path)/socialgraph/comlog/comlogconfig.cpp',
				'<(main_project_path)/socialgraph/comlog/global.h',
  	  ],
  	  'dependencies' : [
  	  ],
  	}, 
  ] #end targets
}

