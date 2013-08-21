{
   'variables': {
     'ICE_PATH':'/opt/Ice-3.3.1',
     'SLICE2CPP_PATH':'<(ICE_PATH)/bin/slice2cpp',
     'BOOST_PATH':'/usr/local',
     'PROTOC_PATH':'',
    },
   'conditions':[
      ['OS=="linux"', {
       'target_defaults': {
         'cflags':['-fPIC', '-g'],
         'defines':['OS_LINUX', 'POSIX', 'NEWARCH'],
         'include_dirs':[
           '..',
           '<(ICE_PATH)/include',
           '<(BOOST_PATH)/include/boost',
           ],
          },
        },
      ],
      ['OS=="win"', {'target_defaults': {},},],
    ],
 'targets': [
  {
   'target_name':'utiladapter',
   'type':'<(library)',
   'include_dirs':[
     '../UtilSlice/slice',
    ],
    'dependencies': [
      'utiladapter_slice'
    ],
    'direct_dependent_settings': {
      'target_conditions': [
        ['_type=="executable"', {
          'conditions':[
            ['OS=="linux"',{
              'libraries': [
                '-L<(ICE_PATH)/lib64', 
                '-lIceUtil', 
                '-lIce', 
                '-lIceGrid', 
                '-lIceBox',
              ], 
            }],
          ],
        }],
      ],
    },
    'export_dependent_settings': [
      'utiladapter_slice',
    ],
    'sources': [
      '../UtilSlice/slice/Util.h',
      '../UtilSlice/slice/XceAlert.h',
      '../UtilSlice/slice/DataWrapper.h',
      '../UtilSlice/slice/DbDescriptor.h',
      '../UtilSlice/slice/MasterSlaveServer.h',
      '../UtilSlice/slice/ObjectCache.h',
      '../UtilSlice/slice/Proxy.h',
      '../UtilSlice/slice/SubjectObserver.h',
      '../UtilSlice/slice/ReplicaCluster.h',
      '../UtilSlice/slice/SvcDescriptor.h',
      '../UtilSlice/slice/TaskManagerAPI.h',
      '../UtilSlice/slice/XceLogger.h',
      '../UtilSlice/slice/XceMonitor.h',
      '../UtilSlice/slice/Util.cpp',
      '../UtilSlice/slice/XceAlert.cpp',
      '../UtilSlice/slice/DataWrapper.cpp',
      '../UtilSlice/slice/DbDescriptor.cpp',
      '../UtilSlice/slice/MasterSlaveServer.cpp',
      '../UtilSlice/slice/ObjectCache.cpp',
      '../UtilSlice/slice/Proxy.cpp',
      '../UtilSlice/slice/SubjectObserver.cpp',
      '../UtilSlice/slice/ReplicaCluster.cpp',
      '../UtilSlice/slice/SvcDescriptor.cpp',
      '../UtilSlice/slice/TaskManagerAPI.cpp',
      '../UtilSlice/slice/XceLogger.cpp',
      '../UtilSlice/slice/XceMonitor.cpp',
    ],
  },
  {
   'target_name':'utiladapter_slice',
   'type':'none',
   'include_dirs':[
     '../UtilSlice/slice',
    ],
    'direct_dependent_settings': {
      'target_conditions': [
        ['_type=="executable"', {
          'conditions':[
            ['OS=="linux"',{
              'libraries': [
                '-L/usr/local/distcache-dev/lib/', 
                #'-lprotobuf',
              ], 
            }],
          ],
        }],
      ],
    },
   'sources': [
      '../UtilSlice/slice/Util.ice',
      '../UtilSlice/slice/XceAlert.ice',
      '../UtilSlice/slice/DataWrapper.ice',
      '../UtilSlice/slice/DbDescriptor.ice',
      '../UtilSlice/slice/MasterSlaveServer.ice',
      '../UtilSlice/slice/ObjectCache.ice',
      '../UtilSlice/slice/Proxy.ice',
      '../UtilSlice/slice/SubjectObserver.ice',
      '../UtilSlice/slice/ReplicaCluster.ice',
      '../UtilSlice/slice/SvcDescriptor.ice',
      '../UtilSlice/slice/TaskManagerAPI.ice',
      '../UtilSlice/slice/XceLogger.ice',
      '../UtilSlice/slice/XceMonitor.ice',
    ],
   'rules': [
        {
         'rule_name':'genproto',
         'extension':'proto',
         'inputs': [],
         'variables': {
            # The protoc compiler requires a proto_path argument with the
            # directory containing the .proto file.
            # There's no generator variable that corresponds to this, so fake it.
           'rule_input_relpath':'../OceProto/proto',
          },
         'outputs': [
           '<(rule_input_relpath)/<(RULE_INPUT_ROOT).pb.h',
           '<(rule_input_relpath)/<(RULE_INPUT_ROOT).pb.cc',
          ],
         'action': [
           '<(PROTOC_PATH)',
           '--proto_path=<(rule_input_relpath)',
           '<(rule_input_relpath)/<(RULE_INPUT_NAME)',
           '--cpp_out=<(rule_input_relpath)',
          ],
         'message':'Generating C++ code from <(RULE_INPUT_PATH)',
        },
        {
         'rule_name':'genslice',
         'extension':'ice',
         'inputs': [],
         'variables': {
           'rule_input_relpath':'../UtilSlice/slice',
          },
         'outputs': [
           '<(rule_input_relpath)/<(RULE_INPUT_ROOT).h',
           '<(rule_input_relpath)/<(RULE_INPUT_ROOT).cpp',
          ],
         'action': [
           '<(SLICE2CPP_PATH)',
           '-I<(ICE_PATH)/slice',
           '-I../OceSlice/slice',
           '-I../UtilSlice/slice',
           '<(RULE_INPUT_PATH)',
           '--output-dir=<(rule_input_relpath)',
          ],
         'message':'Generating C++ code from <(RULE_INPUT_PATH)',
        },
      ],
   },
   {
   'target_name':'oceadapter_slice',
   'type':'none',
   'dependencies': [
      'utiladapter_slice',
   ],
   'sources': [
     '../OceSlice/slice/XceStorm.ice',
     '../OceSlice/slice/UserCache.ice',
     '../OceSlice/slice/BuddyCore.ice',
     '../OceSlice/slice/UserBase.ice',
     '../OceSlice/slice/Ticket.ice',
     '../OceSlice/slice/SocialGraphMessage.ice',
     '../OceSlice/slice/SocialGraphLogic.ice',
     '../OceSlice/slice/SocialGraphWorker.ice',
     '../OceSlice/slice/BuddyByIdCache.ice',
     '../OceSlice/slice/FriendClusterCache.ice',
     '../OceSlice/slice/Tips.ice',
     '../OceSlice/slice/CardCache.ice',
     '../OceSlice/slice/SearchCache.ice',
     '../OceSlice/slice/SearchServer.ice',
     '../OceSlice/slice/SearchModel.ice',
     '../OceSlice/slice/Login.ice',
     '../OceSlice/slice/OzeinContent.ice',
     '../OceSlice/slice/UserDanger.ice',
     '../OceSlice/slice/ContentRcd.ice',
     '../OceSlice/slice/SocialGraphRelation.ice',
     '../OceSlice/slice/IdCache.ice',
    ],
   'rules': [
        {
         'rule_name':'genslice',
         'extension':'ice',
         'inputs': [],
         'variables': {
           'rule_input_relpath':'../OceSlice/slice',
          },
         'outputs': [
           '<(rule_input_relpath)/<(RULE_INPUT_ROOT).h',
           '<(rule_input_relpath)/<(RULE_INPUT_ROOT).cpp',
          ],
         'action': [
           '<(SLICE2CPP_PATH)',
           '-I<(ICE_PATH)/slice',
           '-I../OceSlice/slice',
           '-I../UtilSlice/slice',
           '<(RULE_INPUT_PATH)',
           '--output-dir=<(rule_input_relpath)',
          ],
         'message':'Generating C++ code from <(RULE_INPUT_PATH)',
        },
      ],
   },
   {
   'target_name':'oceadapter',
   'type':'<(library)',
   'include_dirs':[
     '../OceSlice/slice',
     '../UtilSlice/slice',
     '../OceProto/proto/', 
     '/usr/local/distcache/include/', 
    ],
    'dependencies': [
      'oceadapter_slice', 
      'utiladapter_slice',
    ],
    'direct_dependent_settings': {
      'target_conditions': [
        ['_type=="executable"', {
          'conditions':[
            ['OS=="linux"',{
             'include_dirs': [
               '../OceSlice/slice',
               '../UtilSlice/slice',
              ],
              'libraries': [
                '-lmemcached', '-L/usr/local/lib', #'-lprotobuf','-lzookeeper_mt', 
              ], 
            }],
          ],
        }],
      ],
    },
   'sources': [
      '../OceSlice/slice/UserCache.h',
      '../OceSlice/slice/UserCache.cpp',
      '../OceSlice/slice/BuddyCore.h',
      '../OceSlice/slice/BuddyCore.cpp',
      '../OceSlice/slice/XceStorm.h',
      '../OceSlice/slice/XceStorm.cpp',
      '../OceSlice/slice/UserBase.h',
      '../OceSlice/slice/UserBase.cpp',
      '../OceSlice/slice/Ticket.h',
      '../OceSlice/slice/Ticket.cpp',
      '../OceSlice/slice/SocialGraphMessage.h',
      '../OceSlice/slice/SocialGraphMessage.cpp',
      '../OceSlice/slice/SocialGraphLogic.h',
      '../OceSlice/slice/SocialGraphLogic.cpp',
      '../OceSlice/slice/SocialGraphWorker.h',
      '../OceSlice/slice/SocialGraphWorker.cpp',
      '../OceSlice/slice/Tips.h',
      '../OceSlice/slice/Tips.cpp',
      '../OceSlice/slice/BuddyByIdCache.h',
      '../OceSlice/slice/BuddyByIdCache.cpp',
      '../OceSlice/slice/FriendClusterCache.h',
      '../OceSlice/slice/FriendClusterCache.cpp',
      '../OceSlice/slice/CardCache.h',
      '../OceSlice/slice/CardCache.cpp',
      '../OceSlice/slice/SearchCache.h',
      '../OceSlice/slice/SearchCache.cpp',
      '../OceSlice/slice/SearchServer.h',
      '../OceSlice/slice/SearchServer.cpp',
      '../OceSlice/slice/SearchModel.h',
      '../OceSlice/slice/SearchModel.cpp',
      '../OceSlice/slice/Login.h',
      '../OceSlice/slice/Login.cpp',
      '../OceSlice/slice/OzeinContent.h',
      '../OceSlice/slice/OzeinContent.cpp',
      '../OceSlice/slice/UserDanger.h',
      '../OceSlice/slice/UserDanger.cpp',
      '../OceSlice/slice/ContentRcd.h',
      '../OceSlice/slice/ContentRcd.cpp',
      '../OceSlice/slice/SocialGraphRelation.h',
      '../OceSlice/slice/SocialGraphRelation.cpp',
      '../OceSlice/slice/IdCache.h',
      '../OceSlice/slice/IdCache.cpp',
    ],
   },
 ]
}

