
{
  'variables': {
    'library': 'static_library',
    'component': 'static_library',
    'use_system_zlib': '0',
  },
  'conditions': [
    ['OS=="linux"', {
      'target_defaults': {
        'cflags': ['-fPIC', '-g', ],
        'defines': ['OS_LINUX'],
      },
    },],
    ['OS=="win"', {
      'target_defaults': {
        # 'cflags': ['-fPIC', '-g', '-O2',],
        'defines': ['OS_WIN', 'WIN32', 'NOMINMAX', 'UNICODE', '_UNICODE', 'WIN32_LEAN_AND_MEAN', '_WIN32_WINNT=0x0501'],
      },
    },],
  ],

  'targets': [
    {
      'target_name': 'dbc_test',
      'type': 'executable',
      'include_dirs': [
        '..', 
        #'../testing/gtest/include',
        '../third_party',
        #'../third_party/googleurl', # for gurl.h
      ],
      'dependencies': [
        'spdy.gyp:spdy',
        'base.gyp:base',
      ],
      'conditions':[
        ['OS=="linux"', {'libraries': ['-lboost_system', '-lboost_thread', '-lpthread', '-lzookeeper_mt'] }],
        ['OS=="win"', {
          'libraries': ['-lws2_32.lib',],
          'msvs_settings': {
              'VCLinkerTool': {'GenerateDebugInformation': 'true',},
              'VCCLCompilerTool': {'DebugInformationFormat': '3',},
            }, 
        },],
        ],
      'sources': [
'../dbc/dbc_client.cc',
'../dbc/dbc_test.cc',

      ],
    },
  ],
}
