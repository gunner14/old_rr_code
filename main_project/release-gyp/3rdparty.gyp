{
  'variables': {
   'PROTOC_PATH':'/usr/local/distcache-dev/bin/protoc',
   'MAKE_TPL_VARNAMES_PATH':'/usr/local/xce-dev/bin/make_tpl_varnames_h',
   'ICE_PATH':'<!(echo $ICE_HOME)',
   'SLICE2CPP_PATH':'<(ICE_PATH)/bin/slice2cpp',
    'BOOST_PATH':'/usr/local/xce',
  },
 'conditions':[
    ['OS=="linux"', {
     'target_defaults': {
       'cflags':['-fPIC', '-g', '-O2'],
       'defines':['OS_LINUX', 'POSIX', 'NEWARCH'],
       'include_dirs':[
         '<(BOOST_PATH)/include/boost-1_38',
         '..',
         '<(ICE_PATH)/include',
         '/usr/include/mysql',
         '/usr/local/xce/include',
         '/usr/local/distcache/include',
         '/usr/local/xce-dev/include',
         '/usr/local/distcache-dev/include',    # TODO: maybe wrong?
         '/usr/local/distcache-util/include',
         '/opt/mongo-cxx-driver/include',
         ],
        },
      },
    ],
    ['OS=="win"', {'target_defaults': {},},],
  ],
  'targets': [
   {
     'target_name': 'mongoc',
     'type': 'static_library',
     'defines': ['MONGO_HAVE_STDINT'],
     'include_dirs':[
        '../third-party/mongo-c-driver/',
     ],
     'sources': [
       '../third-party/mongo-c-driver/bson.c',
       '../third-party/mongo-c-driver/bson.h',
       '../third-party/mongo-c-driver/gridfs.c',
       '../third-party/mongo-c-driver/gridfs.h',
       '../third-party/mongo-c-driver/md5.c',
       '../third-party/mongo-c-driver/md5.h',
       '../third-party/mongo-c-driver/mongo.c',
       '../third-party/mongo-c-driver/mongo_except.h',
       '../third-party/mongo-c-driver/mongo.h',
       '../third-party/mongo-c-driver/numbers.c',
       '../third-party/mongo-c-driver/platform_hacks.h',
      ],
   },
   {
     'target_name' : 'breakpad',
     'type': 'static_library',
     'defines' : [],
     'include_dirs' : [
       '../third-party/google-breakpad/src/',
       '../third-party/google-breakpad/src/testing/gtest/include',
       '../third-party/google-breakpad/src/testing/include',
     ],
     'sources' : [
       '../third-party/google-breakpad/src/client/linux/handler/exception_handler.cc',
       '../third-party/google-breakpad/src/client/minidump_file_writer.cc',
       '../third-party/google-breakpad/src/client/linux/minidump_writer/minidump_writer.cc',
       '../third-party/google-breakpad/src/client/linux/minidump_writer/linux_dumper.cc',
       '../third-party/google-breakpad/src/client/linux/crash_generation/crash_generation_client.cc',
       '../third-party/google-breakpad/src/client/linux/crash_generation/crash_generation_server.cc',
       '../third-party/google-breakpad/src/common/linux/file_id.cc',
       '../third-party/google-breakpad/src/common/linux/guid_creator.cc',
       '../third-party/google-breakpad/src/common/string_conversion.cc',
       '../third-party/google-breakpad/src/common/convert_UTF.c',
     ],
   },
   {
     'target_name' : 'hiredis',
     'type': 'static_library',
     'defines' : [],
     'include_dirs' : [
       '../third-party/redis-c-driver',
     ],
     'sources' : [
       '../third-party/redis-c-driver/async.c',
       '../third-party/redis-c-driver/async.h',
       '../third-party/redis-c-driver/dict.c',
       '../third-party/redis-c-driver/dict.h',
       '../third-party/redis-c-driver/fmacros.h',
       '../third-party/redis-c-driver/hiredis.c',
       '../third-party/redis-c-driver/hiredis.h',
       '../third-party/redis-c-driver/net.c',
       '../third-party/redis-c-driver/net.h',
       '../third-party/redis-c-driver/sds.c',
       '../third-party/redis-c-driver/sds.h',
     ],
   },
  ]
}
