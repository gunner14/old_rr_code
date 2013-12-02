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
          '<(main_project_path)',
          '<(ice_path)/include',
          '/usr/include/mysql',
          '/usr/local/xce/include',
          '<(boost_path)/include/boost-1_38',
          ],
        'link_settings' : {
          'ldflags': [
            '-Wl,-rpath <(boost_path)/lib',
            '-Wl,-rpath <(ice_path)/lib64',
            '-Wl,-rpath /usr/local/xce-dev/lib64',
            '-Wl,-rpath /usr/local/distcache/lib',
            '-Wl,-rpath /usr/local/xce/lib/',
          ],
          'libraries' : [
            '-L<(ice_path)/lib64',
            '-lIce', '-lIceXML', '-lIceUtil', '-lIceStorm', '-lIceBox', '-lIceGrid', 
            '-L<(boost_path)/lib',
            '-lboost_date_time-gcc41-mt-1_38', '-lboost_system-gcc41-mt-1_38', '-lboost_regex-gcc41-mt-1_38', 
            '-lboost_thread-gcc41-mt-1_38', '-lboost_filesystem-gcc41-mt-1_38',
            '-L/usr/local/xce/lib64',
            '-lmysqlpp', 
            '-llog4cplus',
            '-L/usr/lib64/mysql',
            '-lmysqlclient_r',
            '-licudata', '-licui18n', '-licuuc',
            '-L/usr/local/distcache/lib',
            '-lmemcached', 
            '-L/usr/local/distcache-dev/lib',
            '-lprotobuf', '-lzookeeper_mt',
            '-lstdc++', '-lm', '-lrt','-lpthread',
            #util/cpp/String.h用到了openssl的md5
            '-lcrypto',
          ],
        },
         },
       },
    ],
    ['OS=="win"', 
      {'target_defaults': {},},
    ],
  ],

  'targets' : [
  {
    'target_name' : 'xce_common_slice_ice',
    'type' : 'none',
    'include_dirs' : [
      '<(main_project_path)/OceSlice/slice',
    ],
    'direct_dependent_settings' : {
      'include_dirs' : [
        '<(main_project_path)/OceSlice/slice',
      ],
    },
    'sources' : [
      '<(main_project_path)/OceSlice/slice/Monitor.ice',
    ], 
    'rules' : [
      {
        'rule_name' : 'genslice', 
        'extension':'ice',
        'inputs': [],
        'variables': {
          'rule_input_relpath':'<(main_project_path)/UtilSlice/slice',
         },
        'outputs': [
          '<(rule_input_relpath)/<(RULE_INPUT_ROOT).h',
          '<(rule_input_relpath)/<(RULE_INPUT_ROOT).cpp',
         ],
        'action': [
          '<(slice2pp_path)',
          '-I<(ice_path)/slice',
          '-I<(main_project_path)/UtilSlice/slice',
          '<(RULE_INPUT_PATH)',
          '--output-dir=<(rule_input_relpath)',
         ],
        'message':'Generating C++ code from <(RULE_INPUT_PATH)',
      }
    ],
  }, # end target xce_common_slice_ice

  {
    'target_name' : 'xce_common_slice',
    'type' : 'static_library',
    'dependencies' : [
      'xce_base.gyp:util_slice_ice',
      'xce_common_slice_ice',
    ],
    'sources' : [
      '<(main_project_path)/OceSlice/slice/Monitor.cpp',
      '<(main_project_path)/OceSlice/slice/Monitor.h',
    ],
  }, # end target xce_common_slice

  {
    #放一些被多个工程共同依赖但不需要放到xce_base的库
    'target_name' : 'xce_common',
    'type' : 'static_library',
    'include_dirs' : [
      '<(main_project_path)/UtilSlice/slice',
      '<(main_project_path)/UtilCxx/src',
      '<(main_project_path)/util/cpp',
      '<(main_project_path)/IceExt/src',
      '<(main_project_path)/DbCxxPool/src',
    ],
    'direct_dependent_settings' : {
      'include_dirs' : [
        '<(main_project_path)/UtilCxx/src',
        '<(main_project_path)/util/cpp',
        '<(main_project_path)/IceExt/src',
      ],
    },
    'dependencies' : [
      'xce_base.gyp:util_slice_ice',
      'xce_common_slice_ice',
      'xce_common_slice',
    ],
    'sources' : [
      '<(main_project_path)/OceCxxAdapter/src/MonitorAdapter.cpp',
    ]
  }, # end xce_base
  ],
} 
