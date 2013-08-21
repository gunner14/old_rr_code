{
  'targets': [
    {
      'target_name': 'vldb_server',
      'type': 'executable',
      'msvs_guid': 'FD26E35F-F6A7-402F-885E-4EC57DC62C3A',
      'include_dirs': [
        '..', 
        '../third_party',
        '../bson',
      ],
      'dependencies': [
        'storage.gyp:storage',
        'spdy.gyp:spdy',
        '../bson/bson.gyp:bson',
      ],
      'conditions':[
        ['OS=="linux"', {'libraries': ['-lboost_system', '-lboost_thread', '-lpthread',] }],
        ['OS=="win"', 
          {
            'libraries': ['-lws2_32.lib',],
            'msvs_settings': {
              'VCLinkerTool': {'GenerateDebugInformation': 'true',},
              'VCCLCompilerTool': {'DebugInformationFormat': '3',},
            },
          },
        ],
      ],
      'sources': [
'../vldb/vldb_server.h',
'../vldb/vldb_server.cc',
      ],
    },
  ],
}
