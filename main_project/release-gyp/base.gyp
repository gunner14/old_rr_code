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
         '<(boost_path)/include/boost-1_38',
         '<(main_project_path)',
         ],
        },
      },
    ],
    ['OS=="win"', {'target_defaults': {},},],
  ],
  'targets':[
    {
      'target_name':'base',
      'type':'static_library',
      'direct_dependent_settings': {
        'include_dirs': [
          '<(boost_path)/include/boost-1_38',
        ],
        'target_conditions': [
          ['_type=="executable"', {
            'conditions':[
              ['OS=="linux"',{
                'libraries': [
                  '-L<(boost_path)/lib',
                  '-lboost_regex-gcc41-mt', 
                  '-lboost_date_time-gcc41-mt', 
                  '-lboost_thread-gcc41-mt', 
                  '-lboost_system-gcc41-mt', 
                  '-lboost_filesystem-gcc41-mt', 
                  '-lpthread',
                ], 
              }],
            ],
          }],
        ],
      },
      'sources': [
        '<(main_project_path)/base/asyncall.cc',
        '<(main_project_path)/base/atomicops-internals-x86.cc',
        '<(main_project_path)/base/baseasync.cc',
        '<(main_project_path)/base/common.cc',
        '<(main_project_path)/base/doobs_hash.cc',
        '<(main_project_path)/base/jdbcurl.cc',
        '<(main_project_path)/base/logging.cc',
        '<(main_project_path)/base/md5.cc',
        '<(main_project_path)/base/sha2.cc',
        '<(main_project_path)/base/messagequeue.cc',
        '<(main_project_path)/base/mkdirs.cc',
        '<(main_project_path)/base/network.cc',
        '<(main_project_path)/base/no_exception.cc',
        '<(main_project_path)/base/once.cc',
        '<(main_project_path)/base/pathops.cc',
        '<(main_project_path)/base/pcount.cc',
        '<(main_project_path)/base/signals.cc',
        '<(main_project_path)/base/startuplist.cc',
        '<(main_project_path)/base/stringdigest.cc',
        '<(main_project_path)/base/stringencode.cc',
        '<(main_project_path)/base/thread.cc',
        '<(main_project_path)/base/threadpool.cc',
      ],
    },
    #{
    #  'target_name':'base_test',
    #  'type': 'executable',
    #  'dependencies': [
    #    'gtest.gyp:gtest',
    #    'gtest.gyp:gtest_main',
    #    'base',
    #  ],  
    #  'conditions':[
    #    ['OS=="linux"',{
    #      'link_settings': {
    #        'ldflags': [
    #          '-Wl,-rpath <(boost_path)/lib',
    #        ],
    #      },
    #    }],
    #  ],
    #  'sources': [
    #    #'<(main_project_path)/base/test/string_md5_test.cc'
    #    #'<(main_project_path)/base/test/network_test.cc'
    #    #'<(main_project_path)/base/test/consistenthash_test.cc'
    #    '<(main_project_path)/base/test/string_sha2_test.cc'
    #    #'<(main_project_path)/base/test/hash_test.cc'
    #  ],
    #},
  ],
}
