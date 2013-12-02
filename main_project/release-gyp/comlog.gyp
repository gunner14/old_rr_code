{
  'includes':[
    'global_vars.gyp',
  ],
  'conditions':[
    ['OS=="linux"', {
      'target_defaults': {
        'cflags':['-fPIC', '-g', '-O2'],
        'defines':['OS_LINUX', 'POSIX', 'NEWARCH'],
        'include_dirs':[
        ],
        'link_settings' : {
          'ldflags': [
          ],
          'libraries' : [
          ],
        },
        'dependencies' : [
        ],
      },
     },
    ],
    ['OS=="win"', 
      {'target_defaults': {},},
    ],
  ], # end conditions

  'targets' : [
  {
    'target_name' : 'comlog',
    'type' : 'static_library',
    'defines' : [],
    'sources' : [
	'<(main_project_path)/socialgraph/comlog/comlogaddpender.cpp',
	'<(main_project_path)/socialgraph/comlog/comlogconfig.cpp',
	'<(main_project_path)/socialgraph/comlog/comlogfilter.cpp',
	'<(main_project_path)/socialgraph/comlog/comloglayout.cpp',
	'<(main_project_path)/socialgraph/comlog/comlogmain.cpp',
    ],
	'cflags':['-fPIC', '-g', '-O2'],
	'defines':['OS_LINUX', 'POSIX', 'NEWARCH'],
	'include_dirs':[
	'../../trunk/third-party/log4cplus/include/',
	],
	'link_settings' : {
		'ldflags': [
		],
		'libraries' : [
		'-L../../trunk/third-party/log4cplus/lib/',
		'-llog4cplus',
		],
	},
	'dependencies' : [
	],
  },
  ]
}
