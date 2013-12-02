{
  'includes':[
    '../global_vars.gyp',
  ],
  'conditions':[
    ['OS=="linux"', {
      'target_defaults': {
        'cflags':['-fPIC', '-g', '-O2'],
        'defines':['OS_LINUX', 'POSIX', 'NEWARCH'],
        'include_dirs':[
          '<(main_project_path)',
          '<(ice_path)/include',
          '/usr/include/mysql',
          '/usr/local/xce/include',
          '/usr/local/distcache-dev/include',
          '/usr/local/distcache-util/include',
          '/usr/local/distcache/include',
          '<(boost_path)/include/boost-1_38',
          '<(main_project_path)/DbCxxPool/src',
          '<(main_project_path)/third-party/include/',
          '<(main_project_path)/OceCommon/src',
          
          '<(main_project_path)/third-party/redis-c-driver/',
          '<(main_project_path)/third-party/apr/include/apr-1/',
          '<(main_project_path)/third-party/libactivemq/include/activemq-cpp-3.4.1/',
          '<(main_project_path)/tripod-new/src/cpp/include/',
          '<(main_project_path)/message_pipe/src/cpp',
          

          '<(oce_slice_path)',
          '<(oce_proto_path)',
          '<(adapter_path)',
        ],
        'link_settings' : {
           'libraries' : [
            
             
             '-L../third-party/libactivemq/lib', 
             '-lactivemq-cpp',

       	     '-L../third-party/apr/lib',
             '-L/usr/local/xce-dev/lib64',
             '-lapr-1', '-laprutil-1',

             '-L<(ice_path)/lib64',
             '-lIce', '-lIceXML', '-lIceUtil', '-lIceStorm', '-lIceBox', '-lIceGrid', 
             '-L<(boost_path)/lib',
             '-lboost_date_time-gcc41-mt-1_38', '-lboost_system-gcc41-mt-1_38', '-lboost_regex-gcc41-mt-1_38', 
             '-lboost_thread-gcc41-mt-1_38', '-lboost_filesystem-gcc41-mt-1_38',
             '-L/usr/local/xce/lib64',
             '-lmysqlpp', 
             '-llog4cplus',
             '-L/usr/lib64/mysql',
             '-lmysqlclient_r',
             '-licudata', '-licui18n', '-licuuc',
             '-lstdc++', '-lm', '-lrt','-lpthread',
             '-L/usr/local/distcache-util/lib',
             '-lrdc-client',  
           ],
          'ldflags': [
            '-Wl,-rpath <(boost_path)/lib',
            '-Wl,-rpath <(ice_path)/lib64',
            '-Wl,-rpath /usr/local/xce-dev/lib64',
            '-Wl,-rpath /usr/local/distcache/lib',
            '-Wl,-rpath /usr/local/xce/lib/',
          ],
        },
        'dependencies' : [
          '../oce_slice.gyp:oce_slice',
          '../oce_proto.gyp:oce_proto',
          '../xce_base.gyp:util_slice_ice',
          '../xce_base.gyp:util_slice',
          '../xce_base.gyp:xce_base',
          '../xce_base.gyp:db_cxx_pool',
          '../tripod2.gyp:tripod_core',
        ],
      },
     },
    ],
    ['OS=="win"', 
      {'target_defaults': {},},
    ],
  ],
}
