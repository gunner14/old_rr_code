{
  'variables': {
   #'ICE_PATH':'/opt/Ice-3.3.1',#'<!(echo $ICE_HOME)',
   'TRCACHEPATH':'./protocol',
   #'SLICE2CPP_PATH':'<(ICE_PATH)/bin/slice2cpp',
  },
 'conditions':[
    ['OS=="linux"', {
     'target_defaults': {
       #'cflags': [ '-g', '-O0', '-Wall', '-std=gnu99'],#, '-fast'
       'cxxflags': [ '-g', '-O0', '-Wall'],#, '-fast'
       'defines':['OS_LINUX', 'POSIX', 'NEWARCH', 'NDEBUG', 'HAVE_CONFIG_H',],
       'include_dirs':[
         './protocol',
         '..',
         #'<(ICE_PATH)/include',
         #'/usr/include/mysql',
         #'/usr/local/include/ImageMagick',
       ],
      },
    },
   ],
   ['OS=="win"', {'target_defaults': {},},],
  ],

  'targets': [
  {
    'target_name': 'diskcache', 
    'type': 'executable',
    #'type': 'static_library',
    'include_dirs':[
    '../cwf/src',
    ],
    'dependencies':[
      #'uic.gyp:uic',
    '../cwf/gyp/base3.gyp:base3',
    ],
    'conditions':[
      ['OS=="linux"', {
        'cflags': [ '-g', '-O0', '-Wall', '-std=gnu99'],#, '-fast'
        'libraries': ['-L/usr/local/lib', '-levent', '-lrt'],

        'link_settings': {
         # 'ldflags': [ '-Wl,-rpath /usr/local/lib', ],
        },
     }],
   ],
   'sources': [
      '<(TRCACHEPATH)/memcached.c',
      '<(TRCACHEPATH)/memcached.h',
      '<(TRCACHEPATH)/slabs.c',
      '<(TRCACHEPATH)/slabs.h',
      '<(TRCACHEPATH)/hash.h',
      '<(TRCACHEPATH)/hash.c',
      '<(TRCACHEPATH)/util.h',
      '<(TRCACHEPATH)/util.cc',
      '<(TRCACHEPATH)/items.c',
      '<(TRCACHEPATH)/items.h',
      '<(TRCACHEPATH)/assoc.c',
      '<(TRCACHEPATH)/assoc.h',
      '<(TRCACHEPATH)/daemon.c',
      '<(TRCACHEPATH)/thread.c',
      '<(TRCACHEPATH)/stats.h',
      '<(TRCACHEPATH)/stats.c',
      '<(TRCACHEPATH)/trace.h',
      '<(TRCACHEPATH)/cache.h',
      '<(TRCACHEPATH)/cache.c',
      '<(TRCACHEPATH)/sasl_defs.h',
      'diskmanage.h',
      'diskmanage.cc',
      'disk.h',
      'disk.cc',
      'common_struct.h',
    ],
   },
   {
    'target_name': 'test', 
    'type': 'executable',
    #'type': 'static_library',
    'include_dirs':[
    ],
    'dependencies':[
      #'uic.gyp:uic',
    ],
    'conditions':[
      ['OS=="linux"', {
        'cflags': [ '-g', '-O0', '-Wall'],#, '-fast'
        'cxxflags': [ '-g', '-O0', '-Wall'],#, '-fast'
        'libraries': ['-L/usr/local/lib', '-levent', '-lrt', '-lmemcached', '-lssl', '-lstdc++'],

        'link_settings': {
         # 'ldflags': [ '-Wl,-rpath /usr/local/lib', ],
        },
     }],
   ],
   'sources': [
      'memtest.cc',
    ],
   },
      {
    'target_name': 'scannner', 
    'type': 'executable',
    #'type': 'static_library',
    'include_dirs':[
    ],
    'dependencies':[
      #'uic.gyp:uic',
    ],
    'conditions':[
      ['OS=="linux"', {
        'cflags': [ '-g', '-O0', '-Wall'],#, '-fast'
        'cxxflags': [ '-g', '-O0', '-Wall'],#, '-fast'
        'libraries': ['-L/usr/local/lib', '-levent', '-lrt', '-lmemcached', '-lssl', '-lstdc++'],

        'link_settings': {
         # 'ldflags': [ '-Wl,-rpath /usr/local/lib', ],
        },
     }],
   ],
   'sources': [
      'diskfilescanner.cc',
    ],
   },
  ]
}
