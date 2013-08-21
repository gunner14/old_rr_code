{
  'variables': {
   'ICE_PATH':'/opt/Ice-3.3.1',#'<!(echo $ICE_HOME)',
   'UPLOADPATH':'../upload_new',
   'QUERYSIZEPATH':'../querysize',
   'SLICE2CPP_PATH':'<(ICE_PATH)/bin/slice2cpp',
   'GTEST_PATH':'../testing',
  },
 'conditions':[
    ['OS=="linux"', {
     'target_defaults': {
       'cflags':['-fPIC', '-g'],
       'defines':['OS_LINUX', 'POSIX', 'NEWARCH', 'NDEBUG', 'kNewUpload'],#, 'kPhotoDetectLog'
       'include_dirs':[
         '..',
         '<(ICE_PATH)/include',
         '/usr/include/mysql',
         ],
        },
      },
    ],
    ['OS=="win"', {'target_defaults': {},},],
  ],
  'targets': [
#   { 'target_name': 'blocktest',
#   'type': 'executable',
#   'include_dirs':[
#     '../cwf/src',
#   ],
#   'dependencies':[
#      '../cwf/gyp/base3.gyp:base3',
#   ],
#   'conditions':[
#      ['OS=="linux"', {
#        'libraries': ['-lboost_system', '-lboost_thread', 
#                      '-lmemcached', 
#                      #-L/usr/local/lib -lcurl -L/usr/kerberos/lib64 -lidn -lssl -lcrypto -lldap -lrt -lssl -lcrypto -ldl -lz -lz
#                      '-lbz2', '-lz', '-lm', '-lpthread'],
#
#        'link_settings': {
#          'ldflags': [
#           '-Wl,-rpath /usr/local/lib',
#           #'-Wl,-rpath /usr/kerberos/lib64'
#          ],
#        },
#     }],
#   ],
#   'sources': [
#      '<(UPLOADPATH)/blockmanage.h',
#      '<(UPLOADPATH)/blockmanage.cc',
#      '<(UPLOADPATH)/block_test.cc',
#    ],
#   },
    { 
      'target_name': 'unit_test',
      'type': 'executable',
      'include_dirs':[
        '../cwf/src',
        '<(GTEST_PATH)/gtest/include',
      ],
      'dependencies':[
        '../cwf/gyp/base3.gyp:base3',
        '<(GTEST_PATH)/gtest.gyp:gtest',
      ],
      'cflags': [
        '-fprofile-arcs',
        '-ftest-coverage',
      ],
      'ldflags': [
        '-fprofile-arcs',
      ],
      'libraries': ['-lboost_system', '-lboost_thread', 
                      '-lmemcached', '-lssl', 
                      '-lbz2', '-lz', '-lm', '-lpthread'],

      'link_settings': {
        'ldflags': [
          '-Wl,-rpath /usr/local/lib',
        ],
      },
      'sources': [
        '<(UPLOADPATH)/album_util.cc',
        '<(UPLOADPATH)/album_util_test.cc',
        '<(UPLOADPATH)/util.cpp',
        '<(UPLOADPATH)/util_test.cpp',
        '<(UPLOADPATH)/unit_test.cc',
      ],
    },
    {
      'target_name' : 'test',
      'type': 'none',
      'dependencies': [
        #'unit_test',
      ],
      'actions': [
      {
        'action_name': 'unit_test',
        'inputs': [
          'unit_test',
        ],
        'outputs': [ 'unit_test.out' ],
        'action': [
          '<(PRODUCT_DIR)/unit_test',
        ],
      },
      ],
    },
    {
      'target_name' : 'coverage',
      'type': 'none',
      'dependencies': [
        'test',
      ],
      'actions': [
      {
        'action_name': 'coverage',
        'inputs':[
          '<(UPLOADPATH)/album_util.cc',
        ],
        'outputs' : ['coverage.out'],
        'action' : [
          'python',
          'coverage.py',
          'out/Default/obj.target/upload_new/',
          '<(_inputs)',
          '<@(_outputs)',
        ],
      },
      ]
    },
    {
      'target_name' : 'valgrind',
      'type': 'none',
      'dependencies': [
        #'test',
      ],
      'actions': [
      {
        'action_name': 'valgrind',
        'inputs':[
          'test',
        ],
        'outputs' : ['valgrind.out'],
        'action' : [
          'valgrind',
          '<(PRODUCT_DIR)/unit_test',
        ],
      },
      ]
    },
   { 'target_name': 'upload.fcgi',
   'type': 'executable',
   'include_dirs':[
     '../cwf/src',
     '/usr/local/include/ImageMagick',
   ],
   'dependencies':[
      '../cwf/gyp/base3.gyp:base3',
      'jsoncpp.gyp:json',
      '../cwf/gyp/cwf.gyp:cwf',
      '../cwf/gyp/cwf.gyp:cwfmain',
      'ice.gyp:utiladapter',
      'ice.gyp:oceadapter',
      'dbdesc.gyp:dbpool',
   ],
   'export_dependent_settings': ['jsoncpp.gyp:json'],
   'conditions':[
      ['OS=="linux"', {
        'libraries': ['-lboost_system','-lboost_regex', '-lboost_thread', '-lfcgi', '-lid3',
                      '-lssl' , '-lmemcached',   #'<!@(curl-config --libs)',
                      '<!@(curl-config --libs)',
                      '-lMagick++', '-lMagickWand', '-lMagickCore', 
'-ljpeg', 
                      '-lbz2', '-lz', '-lexiv2', '-lm', '-lpthread'],

        'link_settings': {
          'ldflags': [
           '-Wl,-rpath /usr/local/lib',
           '-Wl,-rpath <(ICE_PATH)/lib64'
          ],
        },
     }],
   ],
   'sources': [
      '../arch_diff/BuddyByIdReplicaAdapter.h',
      '../arch_diff/BuddyByIdReplicaAdapter.cpp',
      '../arch_diff/BuddyCheckHelper.h',
      '../arch_diff/IdCacheReaderAdapter.h',
      '../arch_diff/IdCacheReaderAdapter.cpp',
      '../arch_diff/IpAddr.hpp',
      '../arch_diff/Markable.hpp',
      '../arch_diff/Date.h',
      '../arch_diff/Channel.h',
      '../arch_diff/Channel.cpp',
      '../arch_diff/TaskManager.h',
      '../arch_diff/TaskManager.cpp',
      '../arch_diff/ThreadPoolExecutor.cpp',
      '../arch_diff/ThreadPoolScheduler.cpp',
      '../arch_diff/ClientInterface.h',
      '../arch_diff/ClusterClient.h',
      '../arch_diff/ClusterStateSubscriber.h',
      '../arch_diff/ClusterStateSubscriber.cpp',
      '../arch_diff/Singleton.h',
      '../arch_diff/SbjTopicI.h',
      '../arch_diff/SbjTopicI.cpp',
      '../arch_diff/AdapterI.cpp',
      '../arch_diff/AdapterI.h',
      '../arch_diff/site_xiaonei.h',
      '../arch_diff/site_xiaonei.cc',
      '../upload/download.cc',
      '../upload/download.h',
      '../upload/syncdownload.cc',
      '../upload/syncdownload.h',
      '../upload/url_parse.h',
      '../upload/url_parse.cc',
      '../upload/upload.h',
      '../upload/upload.cc',
      '<(UPLOADPATH)/util_upload.h',
      '<(UPLOADPATH)/util_upload.cc',
      '<(UPLOADPATH)/action_upload.cc',
      '<(UPLOADPATH)/action_upload.h',
      '<(UPLOADPATH)/tickmanager.h',
      '<(UPLOADPATH)/tickmanager.cc',
      '<(UPLOADPATH)/datadispense.cc',
      '<(UPLOADPATH)/musichandler.h',
      '<(UPLOADPATH)/attachmenthandler.h',
      '<(UPLOADPATH)/musichandler.cc',
      '<(UPLOADPATH)/attachmenthandler.cc',
      '<(UPLOADPATH)/registry.cc',
      '<(UPLOADPATH)/imagehelper.h',
      '<(UPLOADPATH)/infconfigparser.h',
      '<(UPLOADPATH)/runtimeconfig.h',
      '<(UPLOADPATH)/text.h',
      '<(UPLOADPATH)/util.h',
      '<(UPLOADPATH)/imagehelper.cpp',
      '<(UPLOADPATH)/infconfigparser.cpp',
      '<(UPLOADPATH)/runtimeconfig.cpp',
      '<(UPLOADPATH)/util.cpp',
      '<(UPLOADPATH)/imagehandler.h',
      '<(UPLOADPATH)/imagehandler.cc',
      '<(UPLOADPATH)/memcache.h',
      '<(UPLOADPATH)/memcache.cc',
      '<(UPLOADPATH)/restoremanage.h',
      '<(UPLOADPATH)/restoremanage.cc',
      '<(UPLOADPATH)/blockmanage.h',
      '<(UPLOADPATH)/blockmanage.cc',
      '<(UPLOADPATH)/downloader.h',
      '<(UPLOADPATH)/downloader.cc',
      '<(UPLOADPATH)/album_util.h',
      '<(UPLOADPATH)/album_util.cc',
    ],
   },
#   { 'target_name': 'querysize.fcgi',
#   'type': 'executable',
#   'include_dirs':[
#     '../cwf/src',
#     '/usr/local/include/ImageMagick',
#   ],
#   'dependencies':[
#      '../cwf/gyp/base3.gyp:base3',
#      'jsoncpp.gyp:json',
#      '../cwf/gyp/cwf.gyp:cwf',
#      '../cwf/gyp/cwf.gyp:cwfmain',
#      'ice.gyp:utiladapter',
#      'ice.gyp:oceadapter'
#   ],
#   'export_dependent_settings': ['jsoncpp.gyp:json'],
#   'conditions':[
#      ['OS=="linux"', {
#        'libraries': ['-lboost_system', '-lboost_thread', '-lfcgi',
#                      '-lssl' , '-lmemcached',   #'<!@(curl-config --libs)',
#                      '<!@(curl-config --libs)',
#                      '-lMagick++', '-lMagickWand', '-lMagickCore', '-ljpeg.ipp', 
#                      '-lbz2', '-lz', '-lm', '-lpthread'],
#
#        'link_settings': {
#          'ldflags': [
#           '-Wl,-rpath /usr/local/lib',
#           '-Wl,-rpath <(ICE_PATH)/lib64'
#          ],
#        },
#     }],
#   ],
#   'sources': [
#      '../arch_diff/site_xiaonei.h',
#      '../arch_diff/site_xiaonei.cc',
#      '<(QUERYSIZEPATH)/photo_upload.h',
#      '<(QUERYSIZEPATH)/photo_upload.cc',
#      '<(QUERYSIZEPATH)/registry.cc',
#      '<(QUERYSIZEPATH)/imagehelper.h',
#      '<(QUERYSIZEPATH)/util.h',
#      '<(QUERYSIZEPATH)/util.cpp',
#      '<(QUERYSIZEPATH)/imagehelper.cpp',
#      '<(QUERYSIZEPATH)/imagehandler.h',
#      '<(QUERYSIZEPATH)/imagehandler.cc',
#      '<(QUERYSIZEPATH)/memcache.h',
#      '<(QUERYSIZEPATH)/memcache.cc',
#    ],
#   },
#   { 'target_name': 'batchqs',
#   'type': 'executable',
#   'include_dirs':[
#     '../cwf/src',
#     '/usr/local/include/ImageMagick',
#   ],
#   'dependencies':[
#      '../cwf/gyp/base3.gyp:base3',
#   ],
#   'conditions':[
#      ['OS=="linux"', {
#        'libraries': ['-lboost_system', '-lboost_thread', '-lboost_filesystem',
#                      '-lssl', 
#                      '-lMagick++', '-lMagickWand', '-lMagickCore', '-ljpeg.ipp', 
#                      '-lbz2', '-lz', '-lm', '-lpthread'],
#
#        'link_settings': {
#          'ldflags': [
#           '-Wl,-rpath /usr/local/lib',
#          ],
#        },
#     }],
#   ],
#   'sources': [
#      '<(QUERYSIZEPATH)/imagehelper.h',
#      '<(QUERYSIZEPATH)/util.h',
#      '<(QUERYSIZEPATH)/util.cpp',
#      '<(QUERYSIZEPATH)/imagehelper.cpp',
#      '<(QUERYSIZEPATH)/imagehandler.h',
#      '<(QUERYSIZEPATH)/imagehandler.cc',
#      '<(QUERYSIZEPATH)/photo_head.h',
#      '<(QUERYSIZEPATH)/main.cc',
#    ],
#   },
#   { 'target_name': 'testjpg',
#   'type': 'executable',
#   'include_dirs':[
#     '../cwf/src',
#     '/usr/local/include/ImageMagick',
#   ],
#   'dependencies':[
#      '../cwf/gyp/base3.gyp:base3',
#   ],
#   'conditions':[
#      ['OS=="linux"', {
#        'libraries': ['-lboost_system', '-lboost_thread', '-lboost_filesystem',
#                      '-lssl', 
#                      '-lMagick++', '-lMagickWand', '-lMagickCore', '-ljpeg.ipp', 
#                      '-lbz2', '-lz', '-lm', '-lpthread'],
#
#        'link_settings': {
#          'ldflags': [
#           '-Wl,-rpath /usr/local/lib',
#          ],
#        },
#     }],
#   ],
#   'sources': [
#      '<(QUERYSIZEPATH)/imagehelper.h',
#      '<(QUERYSIZEPATH)/util.h',
#      '<(QUERYSIZEPATH)/util.cpp',
#      '<(QUERYSIZEPATH)/imagehelper.cpp',
#      '<(QUERYSIZEPATH)/photo_head.h',
#      '<(QUERYSIZEPATH)/test.cc',
#    ],
#   },
  ]
}


