{
  'variables': {
    'JSONPATH': '../third_party/jsoncpp-src-0.5.0',
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
        'defines': ['OS_WIN', 'NOMINMAX', 'UNICODE', '_UNICODE', 'WIN32_LEAN_AND_MEAN', '_WIN32_WINNT=0x0501'],
      },
    },],
  ],
  'targets': [
    {
      'target_name': 'jsontestrunner',
      'type': 'executable',
      'dependencies': [
        'json',
      ],
      'sources': [
        '<(JSONPATH)/src/jsontestrunner/main.cpp',
      ],
      'include_dirs': [
        '<(JSONPATH)/include',
      ],
    },
    {
      'target_name': 'jsontest',
      'type': 'executable',
      'dependencies': [
        'json',
      ],
      'sources': [
        '<(JSONPATH)/src/test_lib_json/jsontest.cpp',
        '<(JSONPATH)/src/test_lib_json/jsontest.h',
        '<(JSONPATH)/src/test_lib_json/main.cpp',
      ],
      'include_dirs': [
        '<(JSONPATH)/include',
      ],
    },
    {
      'target_name': 'json',
      'type': 'static_library',
      'sources': [
        '<(JSONPATH)/src/lib_json/bsontypes.h',
        '<(JSONPATH)/src/lib_json/json_batchallocator.h',
        '<(JSONPATH)/src/lib_json/json_internalarray.inl',
        '<(JSONPATH)/src/lib_json/json_internalmap.inl',
        '<(JSONPATH)/src/lib_json/json_reader.cpp',
        '<(JSONPATH)/src/lib_json/json_value.cpp',
        '<(JSONPATH)/src/lib_json/json_valueiterator.inl',
        '<(JSONPATH)/src/lib_json/json_writer.cpp',
      ],
      'include_dirs': [
        '<(JSONPATH)/include',
      ],
      'direct_dependent_settings': { 
         'include_dirs': ['<(JSONPATH)/include',],
      },
    },
  ],
}
