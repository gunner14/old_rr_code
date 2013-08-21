{
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
      'target_name': 'bsondemo',
      'type': 'executable',
      'dependencies': [
        'bson',
      ],
      'sources': [
        '../bson/bsondemo/bsondemo.cpp',
      ],
      'include_dirs': [
        '..',
      ],
    },
    {
      'target_name': 'bson_unittest',
      'type': 'executable',
      'dependencies': [
        # 'bson',
        'base.gyp:base',
        '../testing/gtest.gyp:gtestmain',
      ],
      'sources': [
        '../bson/builder_test.cc',
        '../bson/element_test.cc',
        '../bson/bson.cc',
        '../bson/util/base64.cpp',
        '../bson/json.cpp',
        '../bson/json.h',
      ],
      'include_dirs': [
        '..',
        '../testing/gtest/include',
      ],
    },
    {
      'target_name': 'bson',
      'type': '<(library)',
      'sources': [
        '../bson/bsontypes.h',
        '../bson/bsonelement.h',
        '../bson/builder.h',
        '../bson/util/misc.h',
        
        '../bson/bsonobj.h',
        '../bson/bson.h',
        
        '../bson/db/json.h',
        '../bson/db/json.cpp',
        '../bson/db/jsobjmanipulator.h',
        '../bson/db/jsobj.h',
        '../bson/db/jsobj.cpp',

      ],
      'include_dirs': [
        '.',
        '..',
      ],
    },
  ],
}
