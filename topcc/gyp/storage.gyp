{
  'variables': {
    'library': 'static_library',
  },
  'targets': [
    {
      'target_name': 'storage',
      'type': 'static_library',
      'msvs_guid': 'A5A35681-7B4E-4060-AAAA-30620CA0AAAA',
      'include_dirs': ['..',],
      'dependencies': [
        'base.gyp:base',
        'db.gyp:db',
        '../third_party/zlib/zlib.gyp:zlib',
      ],
      'conditions':[
        ['OS=="linux"', {
          'cflags': ['-fPIC', '-g','-O2' ],
          'include_dirs': ['../third_party/db-stable/build_unix'],
          'defines':['_GNU_SOURCE', '_REENTRANT', 'OS_LINUX'],
        }],
        ['OS=="win"', {
          'include_dirs': ['../third_party/db-stable/build_windows'],
          'defines': ['OS_WIN', 'NOMINMAX', 'UNICODE', '_UNICODE', 'WIN32_LEAN_AND_MEAN', '_WIN32_WINNT=0x0501'],
          'msvs_settings': {
              'VCLinkerTool': {'GenerateDebugInformation': 'true',},
              'VCCLCompilerTool': {'DebugInformationFormat': '3',},
            },
        }],
      ],
      'sources': [
        '../storage/vldb.h',
        '../storage/pack.h',
        '../storage/idx.cc',
        '../storage/data.cc',
      ],
    },
    {
      'target_name': 'storage_test',
      'type': 'executable',
      'cflags': [ '-g' ],
      'msvs_guid': 'D8EC380E-CD33-4634-AE71-8A181A43BBBB',
      'include_dirs': ['..', '../testing/gtest/include'],
      'dependencies': [
        'storage',
        '../testing/gtest.gyp:gtestmain'
      ],
      'libraries': ['-lpthread'],
      'sources': [
        '../storage/vldb_test.cc',
      ],
    },
  ],
}
