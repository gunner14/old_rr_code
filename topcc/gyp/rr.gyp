{
  'variables': {
   #'ICE_PATH':'/opt/Ice-3.3.1',#'<!(echo $ICE_HOME)',
   'RRPATH':'../rrimage',
   #'SLICE2CPP_PATH':'<(ICE_PATH)/bin/slice2cpp',
  },
 'conditions':[
    ['OS=="linux"', {
     'target_defaults': {
       'cflags': [ '-g',  '-fpermissive', '-Wall'],
       'defines':['OS_LINUX', 'POSIX', 'NEWARCH', 'NDEBUG'],#, 'kPhotoDetectLog'
       'include_dirs':[
         '..',
         #'<(ICE_PATH)/include',
         #'/usr/include/mysql',
         '/usr/local/include/ImageMagick',
       ],
      },
    },
   ],
   ['OS=="win"', {'target_defaults': {},},],
  ],

  'targets': [
#  {
#    'target_name': 'rr',
#    'type': 'executable',
#    'include_dirs':[
#      '/opt/intel/ipp/include',
#      '/opt/intel/uic/io/uic_io/include',
#      '/opt/intel/uic/core/uic/include',
#      '/opt/intel/uic/codec/image/jpeg',
#      '/usr/local/include/ImageMagick',
#      '/usr/local/include/freetype2',
#    ],
#    'dependencies':[
#      'uic.gyp:uic',
#    ],
#    'conditions':[
#      ['OS=="linux"', {
#        'libraries': ['-lippj', '-lippi', '-luic_io', '-luic_jpeg','-luic_core', '-lirc',
#           '-L/usr/local/lib',
#           '-L/opt/intel/uic/bin', '-L/opt/intel/ipp/lib/intel64',
#           '-lMagick++', '-lMagickWand', '-lMagickCore','-lfreetype',
#           '-lbz2', '-lz', '-lm',
#        ],
#
#        'link_settings': {
#          'ldflags': [
#           #'-Wl,-rpath /usr/local/lib',
#           #'-Wl,-rpath /opt/intel/lib',
#           #'-Wl,-rpath /opt/intel/ipp/lib/intel64'
#          ],
#        },
#     }],
#   ],
#   'sources': [
#      '<(RRPATH)/jpg.h',
#      '<(RRPATH)/jpg.cc',
#      '<(RRPATH)/gif.h',
#      '<(RRPATH)/gif.cc',
#      '<(RRPATH)/unit.cc',
#    ],
#   },
  {
    'target_name': 'rrtest',
    'type': 'executable',
    'include_dirs':[
      '../cwf/src',
      '/opt/intel/ipp/include',
      '/usr/local/include/ImageMagick',
      '/usr/local/include/freetype2',
      #'/opt/intel/uic/core/uic/include',
      #'/opt/intel/uic/codec/image/jpeg',
    ],
    'dependencies':[
      'uic.gyp:uic',
    ],
    'conditions':[
      ['OS=="linux"', {
        'libraries': ['-lpthread', '-lippch', '-lippdc', '-lippcc', '-lippcv', '-lippj', '-lippi', '-lipps', '-lippcore',
           '-L/usr/local/lib',
           '-lMagick++', '-lMagickWand', '-lMagickCore','-lfreetype',
           #'-L/opt/intel/lib/intel64',
           #'-L/opt/intel/ipp/lib/intel64'
        ],

        'link_settings': {
          'ldflags': [
           #'-Wl,-rpath /usr/local/lib',
           #'-Wl,-rpath /opt/intel/lib',
           #'-Wl,-rpath /opt/intel/ipp/lib/intel64'
          ],
        },
     }],
   ],
   'sources': [
      '<(RRPATH)/jpg.h',
      '<(RRPATH)/jpg.cc',
      '<(RRPATH)/png.h',
      '<(RRPATH)/png.cc',
      '<(RRPATH)/gif.h',
      '<(RRPATH)/gif.cc',
      '<(RRPATH)/unit.cc',
      '<(RRPATH)/ftstring.h',
      '<(RRPATH)/watermark_param.h',
      '<(RRPATH)/bmploader/cfile.h',
      '<(RRPATH)/bmploader/bmploader.h',
      '<(RRPATH)/bmploader/bmploader.cc',
      '<(RRPATH)/bmploader/ippiImage.h',
      '<(RRPATH)/bmploader/ippiImage.cpp',
      '<(RRPATH)/bmploader/ippiImageStore.h',
      '<(RRPATH)/bmploader/ippiImageStore.cpp',
    ],
   },
  {
    'target_name': 'rrimage.fcgi',
    'type': 'executable',
    'include_dirs':[
      '../cwf/src',
      '/opt/intel/ipp/include',
      '/usr/local/include/ImageMagick',
      '/usr/local/include/freetype2',
      #'/opt/intel/uic/io/uic_io/include',
      #'/opt/intel/uic/core/uic/include',
      #'/opt/intel/uic/codec/image/jpeg',
    ],
    'dependencies':[
      '../cwf/gyp/base3.gyp:base3',
      '../cwf/gyp/cwf.gyp:cwf',
      '../cwf/gyp/cwf.gyp:cwfmain',
      'uic.gyp:uic',
      #'ice.gyp:utiladapter',
      #'ice.gyp:oceadapter',
      #'dbdesc.gyp:dbpool',
    ],
    #'export_dependent_settings': ['jsoncpp.gyp:json'],
    'conditions':[
      ['OS=="linux"', {
        'libraries': ['-lboost_system', '-lboost_thread', '-lippch_l', '-lippdc_l', '-lippcc_l', '-lippcv_l', '-lippj_l', '-lippi_l', '-lipps_l', '-lippcore_l',
           '-L/usr/local/lib',
           '-lMagick++', '-lMagickWand', '-lMagickCore','-lfreetype',
           #'-L/opt/intel/lib/intel64',
           #'-L/opt/intel/ipp/lib/intel64'
        ],

        'link_settings': {
          'ldflags': [
           '-Wl,-rpath /usr/local/lib',
           '-Wl,-rpath /opt/intel/lib',
           '-Wl,-rpath /opt/intel/ipp/lib/intel64'
          ],
        },
     }],
   ],
   'sources': [
      '<(RRPATH)/jpg.h',
      '<(RRPATH)/jpg.cc',
      '<(RRPATH)/png.h',
      '<(RRPATH)/png.cc',
      '<(RRPATH)/gif.h',
      '<(RRPATH)/gif.cc',
      '<(RRPATH)/action_runimage.cc',
      '<(RRPATH)/registry.cc',
      '<(RRPATH)/action_runimage.h',
      '<(RRPATH)/base64.h',
      '<(RRPATH)/base64.cpp',
      '<(RRPATH)/ftstring.h',
      '<(RRPATH)/watermark_param.h',
      '<(RRPATH)/bmploader/cfile.h',
      '<(RRPATH)/bmploader/bmploader.h',
      '<(RRPATH)/bmploader/bmploader.cc',
      '<(RRPATH)/bmploader/ippiImage.h',
      '<(RRPATH)/bmploader/ippiImage.cpp',
      '<(RRPATH)/bmploader/ippiImageStore.h',
      '<(RRPATH)/bmploader/ippiImageStore.cpp',
      '../upload/syncdownload.cc',
      '../upload/url_parse.h',
      '../upload/url_parse.cc',
    ],
   }
  ]
}
