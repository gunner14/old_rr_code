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
      'target_name': 'spdy',
      'type': 'static_library',
      'msvs_guid': 'A5A35681-7B4E-4060-AB72-30620CA0D8C8',
      'include_dirs': [
        '..',
  '../third_party',
  #'../third_party/googleurl',
      ],
      'dependencies': [
        'base.gyp:base',
        '../third_party/zlib/zlib.gyp:zlib',
       # 'googleurl.gyp:googleurl',
      ],
      'conditions':[
        #['OS=="linux"', {'libraries': ['-lboost_system', '-lboost_thread', '-lpthread', '-lzookeeper_mt'] }],
        ['OS=="win"', {
          'msvs_settings': {
            'VCLinkerTool': {'GenerateDebugInformation': 'true',},
            'VCCLCompilerTool': {'DebugInformationFormat': '3',},
          },
        },],
      ],
      'sources': [
'../net/spdy/spdy_bitmasks.h',
'../net/spdy/spdy_frame_builder.cc',
'../net/spdy/spdy_frame_builder.h',
'../net/spdy/spdy_framer.cc',
'../net/spdy/spdy_framer.h',
'../net/spdy/spdy_protocol.h',
#'../net/base/net_util.h',
#'../net/base/net_util.cc',
#'../net/http/http_byte_range.cc',
#'../net/http/http_request_headers.h',
#'../net/http/http_request_headers.cc',
#'../net/http/http_util.h',
#'../net/http/http_byte_range.h',
#'../net/http/http_util.cc',
#'../net/http/http_request_info.h',
#'../net/http/http_request_info.cc',
'../net/spdy_server.cc',
'../net/spdy_server.h',
'../net/spdy_sess.cc',
'../net/spdy_sess.h',
'../net/url_parse.h',
'../net/spdy_client.cc',
'../net/spdy_client.h',
'../net/config_client.h',
'../net/config_client.cc',
'../net/spdy_registry.h',
'../net/spdy_registry.cc',
'../net/spdy_method.cc',
'../net/spdy_method.h',
'../net/spdy_response.h',
'../net/spdy_response.cc',
      ],
    },
    {
      'target_name': 'spdy_test',
      'type': 'executable',
      'msvs_guid': 'D8EC380E-CD33-4634-AE71-8A181A433DC9',
      'include_dirs': ['..', '../testing/gtest/include',],
      'dependencies': [
        'spdy',
        'base.gyp:base',
        '../testing/gtest.gyp:gtestmain',
      ],
      'conditions':[
        ['OS=="linux"', {'libraries': ['-lboost_system', '-lboost_thread', '-lpthread',] }],
        
        ['OS=="win"', {
        'libraries': ['-lws2_32.lib',],
        'msvs_settings': {
              'VCLinkerTool': {'GenerateDebugInformation': 'true',},
              'VCCLCompilerTool': {'DebugInformationFormat': '3',},
            },
        }],
      ],
      'sources': [
       # '../net/spdy/spdy_framer_test.cc',
       # '../net/spdy/spdy_protocol_test.cc',
       # '../net/http/http_request_headers_unittest.cc',
      ],
    }, 
    {
      'target_name': 'spdy_sess_test',
      'type': 'executable',
      'msvs_guid': 'D8EC380E-CD33-4634-AE71-8A181A433DC9',
      'include_dirs': ['..', '../testing/gtest/include',],
      'dependencies': [
        'spdy',
        'base.gyp:base',
        '../testing/gtest.gyp:gtestmain',
      ],
      'conditions':[
        ['OS=="linux"', {'libraries': ['-lboost_system', '-lboost_thread', '-lpthread',] }],
        
        ['OS=="win"', {
        'libraries': ['-lws2_32.lib',],
        'msvs_settings': {
              'VCLinkerTool': {'GenerateDebugInformation': 'true',},
              'VCCLCompilerTool': {'DebugInformationFormat': '3',},
            },
        }],
      ],
      'sources': [
        '../net/spdy_sess_test.cc',
      ],
    },
    {
      'target_name': 'spdy_fetch',
      'type': 'executable',
      'msvs_guid': '2F0F6EA7-10FB-4A3D-9969-691CF562EA76',
      'include_dirs': [
        '..', 
        '../testing/gtest/include',
        '../third_party',
        #'../third_party/googleurl', # for gurl.h
      ],
      'dependencies': [
        'spdy',
        'base.gyp:base',
      ],
      'conditions':[
#'/usr/local/lib/libzookeeper_mt.a',
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
'../net/spdy_fetch.cc',
      ],
    },
    {
      'target_name': 'echo_server',
      'type': 'executable',
      'msvs_guid': 'FD26E35F-F6A7-402F-885E-4EC57DC62C2A',
      'include_dirs': [
        '..', 
        '../third_party',
       # '../third_party/googleurl', # for gurl.h
      ],
      'dependencies': [
        'spdy',
        'base.gyp:base',
      ],
      'conditions':[
        ['OS=="linux"', {'libraries': ['-lboost_system', '-lboost_thread', '-lpthread',] }],
        ['OS=="win"', 
          {
            'libraries': ['-lws2_32.lib'],
            'msvs_settings': {
              'VCLinkerTool': {'GenerateDebugInformation': 'true',},
              'VCCLCompilerTool': {'DebugInformationFormat': '3',},
            },
          },
        ],
      ],
      'sources': [
'../net/echo_server.cc',
      ],
    },
  ],
}
