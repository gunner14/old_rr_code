{
  'variables': {
   'ICE_PATH':'/opt/Ice-3.3.1',
   'SLICE2CPP_PATH':'<(ICE_PATH)/bin/slice2cpp',
  },
 'conditions':[
    ['OS=="linux"', {
     'target_defaults': {
       'cflags':['-g', '-O2'],
       'defines':['OS_LINUX', 'POSIX', 'NEWARCH'],
       'include_dirs':[
         '..',
         '<(ICE_PATH)/include',
         '/usr/local/mysql/include',
         '/usr/local/include', # for mysql++
         ],
        },
      },
    ],
    ['OS=="win"', {'target_defaults': {},},],
  ],
  'targets': [
   {
   'target_name': 'dbpool',
   'type': '<(library)',
   'include_dirs':[
      '../IceExt/src',
      '../UtilCxx/src',
      '../UtilSlice/slice',
      '../OceSlice/slice',
      '../DbCxxPool/src',
     '../cwf/src',
   ],
   'dependencies': [
      '../cwf/gyp/base3.gyp:base3',
      'ice.gyp:utiladapter',
      'ice.gyp:oceadapter'
   ],
   'direct_dependent_settings': {
     'target_conditions': [
       ['_type=="executable"', {
         'conditions':[
           ['OS=="linux"',{
             'libraries': [
               '-L/usr/local/mysql/lib', '-lmysqlclient',
               '-L/usr/local/lib', '-lmysqlpp',
             ], 
           }],
         ],
       }],
     ],
   },
   'sources': [
     '../dbpool/connectionpoolmanager.cpp',
     '../dbpool/connectionholder.cpp',
     '../dbpool/connectionpool.cpp',
     '../dbpool/connectionquery.cpp',
     '../dbpool/queryrunner.cpp',
     '../dbpool/connectionpool.h',
     '../dbpool/connectionpoolmanager.h',
     '../dbpool/queryrunner.h',
     '../dbpool/connectionholder.h',
     '../dbpool/connectionquery.h',
    ],
   },
  ]
}

